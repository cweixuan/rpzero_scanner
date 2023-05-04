use redis::{Client, cmd, Connection};
extern crate nalgebra as na;
use na::{DMatrix, DVector};

type RedisStream = Vec<Vec<(String, Vec<Vec<(String, Vec<(String, String)>)>>)>>;
type RSSIData = Vec<(String, String)>;

#[derive(Debug)]
struct LocaleResult{
 status: i32, 
 pos: [f64; 3]
}

struct NodeCoord{
    pos: [f64; 3]
}

// each stream message will have the following format: 
// <mac addr>, vec![(<time>_<node>, <rssi>),...,]

// X read stream from redis 
// X check if MAC address is in redis stream 
// X check how many data points returned by redis
// X check if there is at least 3 valid data points (RSSI cutoff, same floor)
// X convert RSSI readings to distance 
// X get coordinates of MAC using multilateration 
// return value via preferred method
fn main() {
    let client = Client::open("redis://127.0.0.1/").unwrap();
    let con: Connection = client.get_connection().unwrap();
    
    let name_stream: &str = "main_stream";

    let result = handle_connection(con, name_stream);

    println!("{:?}", result);
}

fn handle_connection(mut con: Connection, name_stream: &str) -> LocaleResult {
    let mut raw_data: RSSIData = vec![];
    let mut clean_data: RSSIData = vec![]; 
    let mut result = LocaleResult{
        status: 0,
        pos: [0., 0., 0.],
    };

    let test_data: RSSIData = vec![("node2".to_string(), "-40".to_string()),
    ("node3".to_string(), "-10".to_string()),
    ("node4".to_string(), "-60".to_string()),
    ("node1".to_string(), "-10".to_string()),
    ("node5".to_string(), "-80".to_string())]; 

    (raw_data, result) = get_data(&mut con, name_stream);
    if result.status == 0 {
        //let (clean_data, result) = qualify_data(raw_data);
        (clean_data, result) = qualify_data(&mut con, test_data);
        
        if result.status == 0 {
            result = multilateration(&mut con, clean_data);
        }
    }

    result
}

fn get_data(con: &mut Connection, mac: &str) -> (RSSIData, LocaleResult) {
    let mut result = LocaleResult{
        status: 0,
        pos: [0., 0., 0.],
    };
    let mut raw_data: RSSIData = vec![];

    let value: RedisStream = cmd("XREAD")
        .arg("COUNT").arg("1")
        .arg("STREAMS").arg(mac).arg("0")
        .query(con).unwrap();


    if value.len() == 0 {
        result.status = 1;
    }
    else {
        for node in value[0][0].1[0][0].1.iter(){
            raw_data.push(node.clone());
        }
    }


    (raw_data, result)
}

// todo: set cutoff as global const
fn qualify_data(con: &mut Connection, data: RSSIData) -> (RSSIData, LocaleResult) {
    let cutoff: i32 = -65;
    let mut result = LocaleResult{
        status: 0,
        pos: [0., 0., 0.],
    }; 
    let mut clean_data: RSSIData = vec![];

    if data.len() < 3 {
        result.status = 2;
    }
    else {
        // remove nodes not from the same floor 
        let filter_data = filter_floor(con, data.clone());
        println!("{:?}", filter_data);

        for node in filter_data.iter() {
            let rssi: i32 = node.1.parse::<i32>().unwrap();

            if rssi > cutoff {
                clean_data.push((node.0.to_string(), node.1.to_string()));
            }
        }

        if clean_data.len() < 3 {
            result.status = 3;
        }
        else {
            result.status = 0;
        }
    }

    (clean_data, result)
}

fn filter_floor(con: &mut Connection, data: RSSIData) -> RSSIData {
    let mut floors: Vec<(i32, i32, &str)> = vec![];

    for node in data.iter() {
        let rssi: i32 = node.1.parse::<i32>().unwrap();
        
        let node_name = &node.0;

        let value: String = cmd("HGET")
            .arg("node_coords").arg(node_name)
            .query(con).unwrap();

        let z_coord = value.split(";").map(|c| c.parse::<i32>().unwrap()).collect::<Vec<i32>>()[2];


        floors.push((rssi, z_coord, &node_name));
    }
    
    let mut tally = [(0, 2), (0, 3), (0, 4)];
    for tuple in floors.iter() {
        match tuple.1 {
            2 => tally[0].0 += 1,
            3 => tally[1].0 += 1, 
            4 => tally[2].0 += 1,
            _ => ()
        }
    }

    tally.sort(); 

    let floor: i32 = tally[2].1;

    let mut filter_data: RSSIData = vec![];

    for node in data.iter(){
        let node_name = &node.0;
        let mut flag: bool = false;

        for tuple in floors.iter() {
            if tuple.1 != floor {
                if tuple.2 == node_name {
                flag = true;
                }
            }
        }
        if flag == false {
            filter_data.push(node.clone());
        }
    }

    filter_data
}

// todo: 
// set measured power as global const
// set n as global const
fn rssi_to_distance(rssi: i32) -> f64 {
    let measured_power: i32 = -60;
    let n: i32 = 2;

    let distance: f64 = f64::powi(10., (rssi - measured_power)/(-10*n));

    distance
}

// X = (A^T * A)^-1 * A^T * b
// function based on this paper
// https://www.mdpi.com/1424-8220/18/9/2820
fn multilateration(con: &mut Connection, data: RSSIData) -> LocaleResult {
    let mut result = LocaleResult {
        status: 0,
        pos: [0., 0., 0.],
    };
    
    let mut coords: Vec<NodeCoord> = vec![];
    // fill coords in same order as data

    for node in data.iter(){
        let node_name = &node.0;

        let value: String = cmd("HGET")
            .arg("node_coords").arg(node_name)
            .query(con).unwrap();

        let coord = value.split(";").map(|c| c.parse::<f64>().unwrap()).collect::<Vec<f64>>();

        let array = [coord[0], coord[1], coord[2]];
        //println!("{:?}", array);
        let wrap = NodeCoord {
            pos: array,
        };
        
        coords.push(wrap);
    }

    let len: usize = coords.len();

    let mut a = DMatrix::identity(len, 2);
    let mut b = DVector::identity(len);
    
    for row in 0..len{
        for col in 0..2{
            a[(row, col)] = 2.0 * (coords[row].pos[col] - coords[len-1].pos[col]);
        }
        let xi_2 = coords[row].pos[0].powi(2);
        let xm_2 = coords[len-1].pos[0].powi(2);

        let yi_2 = coords[row].pos[1].powi(2);
        let ym_2 = coords[len-1].pos[1].powi(2);

        let di_2 = rssi_to_distance(data[row].1.parse::<i32>().unwrap()).powi(2);
        let dm_2 = rssi_to_distance(data[len-1].1.parse::<i32>().unwrap()).powi(2);

        b[(row)] = xi_2 - xm_2 + yi_2 - ym_2 + di_2 - dm_2;

        //println!("xi_2: {}, xm_2: {}, yi_2: {}, ym_2: {}, di_2: {}, dm_2: {}", xi_2, xm_2, yi_2, ym_2, di_2, dm_2);
        //println!("b[row]: {}", b[(row)]);
    }

    let x = (a.transpose() * a.clone()).try_inverse().unwrap() * a.transpose() * b;

    //println!("x[0]: {}, x[1]: {}", x[(0)], x[(1)]);

    result.pos[0] = x[(0)];
    result.pos[1] = x[(1)]; 

    result
}




