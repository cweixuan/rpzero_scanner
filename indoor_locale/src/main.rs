use redis::{Client, cmd, Connection};
extern crate nalgebra as na;
use na::{DMatrix, DVector};

type RedisStream = Vec<Vec<(String, Vec<(String, String)>)>>;
type RSSIData = Vec<(String, String)>;

// Status OK is not represented here as the functions just return the appropriate value
#[derive(Debug)]
enum Status {
    MACNotFound, 
    LackOfRSSIData,
    RSSINotInRange,
}

#[derive(Debug)]
struct Locale(f64, f64, f64);
struct NodeCoord(f64, f64, f64);

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
    
    let name_stream: &str = "60bbcd6af421";

    let result = handle_connection(con, name_stream);

    println!("{:?}", result);
}

fn handle_connection(mut con: Connection, name_stream: &str) -> Result<Locale, Status> {
    let raw_data = get_data(&mut con, name_stream)?;
    let clean_data = qualify_data(&mut con, raw_data, -99)?;
    let result = multilateration(&mut con, clean_data)?;

    Ok(result)
}   

fn get_data(con: &mut Connection, mac: &str) -> Result<RSSIData, Status> {
    let mut raw_data: RSSIData = vec![];

    let value: RedisStream  = cmd("XREVRANGE")
        .arg(mac).arg("+").arg("-").arg("COUNT").arg("1")
        .query(con).unwrap();
    
    if value.len() == 0 {
        return Err(Status::MACNotFound);
    }
    
    for node in value[0][0].1.iter(){
        raw_data.push(node.clone());
    }

    Ok(raw_data)
}

fn qualify_data(con: &mut Connection, data: RSSIData, cutoff: i32) -> Result<RSSIData, Status> {
    let mut clean_data: RSSIData = vec![];

    if data.len() < 3 {
        return Err(Status::LackOfRSSIData);
    }

    // remove nodes not from the same floor 
    let filter_data = filter_floor(con, data.clone());
    //println!("{:?}", filter_data);

    for node in filter_data.iter() {
        let rssi: i32 = node.1.parse::<i32>().unwrap();

        if rssi > cutoff {
            clean_data.push((node.0.to_string(), node.1.to_string()));
        }
    }

    if clean_data.len() < 3 {
        return Err(Status::RSSINotInRange)
    }

    Ok(clean_data)
}

fn filter_floor(con: &mut Connection, data: RSSIData) -> RSSIData {
    type ZCoord = i32;
    type NodeName<'a> = &'a str;

    let mut node_floors: Vec<(ZCoord, NodeName)> = vec![];

    // populate floors nodes are found in
    for node in data.iter() {
        //let rssi: i32 = node.1.parse::<i32>().unwrap();
        
        let node_name = &node.0;
        
        //println!("{}", node_name);
        let value: String = cmd("HGET")
            .arg("node_coords").arg(node_name)
            .query(con).unwrap();

        let z_coord = value.split(";").map(|c| c.parse::<i32>().unwrap()).collect::<Vec<i32>>()[2];

        node_floors.push((z_coord, &node_name));
    }
    
    // find common floor
    let mut tally = [(0, 2), (0, 3), (0, 4)];
    for node_floor in node_floors.iter() {
        let z_coord = node_floor.0;

        match z_coord {
            2 => tally[0].0 += 1,
            3 => tally[1].0 += 1, 
            4 => tally[2].0 += 1,
            _ => ()
        }
    }

    tally.sort(); 
    let common_floor: i32 = tally[2].1;

    // pick nodes on same floor
    let mut filtered_data: RSSIData = vec![];

    'a: for node in data.iter(){
        let node_name = &node.0;

        // if node is not in common floor, dont add to filtered_data
        for (floor, name) in node_floors.iter() {
            if  name == node_name && *floor != common_floor{
                continue 'a;
            }
        }

        filtered_data.push(node.clone());
    }

    filtered_data
}

fn rssi_to_distance(rssi: i32, measured_power: i32, n: i32) -> f64 {
    let distance: f64 = f64::powi(10., (rssi - measured_power)/(-10*n));

    distance
}

// X = (A^T * A)^-1 * A^T * b
// function based on this paper
// https://www.mdpi.com/1424-8220/18/9/2820
fn multilateration(con: &mut Connection, data: RSSIData) -> Result<Locale, Status> {
    let mut coords: Vec<NodeCoord> = vec![];
   
    // fill coords in same order as data
    for node in data.iter(){
        let node_name = &node.0;

        let value: String = cmd("HGET")
            .arg("node_coords").arg(node_name)
            .query(con).unwrap();

        let coord = value.split(";").map(|c| c.parse::<f64>().unwrap()).collect::<Vec<f64>>();

        let array = [coord[0], coord[1], coord[2]];
        
        coords.push(NodeCoord(array[0], array[1], array[2]));
    }

    let len: usize = coords.len();

    let mut a = DMatrix::identity(len, 2);
    let mut b = DVector::identity(len);
    
    for row in 0..len{
        a[(row, 0)] = 2.0 * (coords[row].0 - coords[len-1].0);
        a[(row, 1)] = 2.0 * (coords[row].1 - coords[len-1].1);
        a[(row, 2)] = 2.0 * (coords[row].2 - coords[len-1].2);

        let xi_2 = coords[row].0.powi(2);
        let xm_2 = coords[len-1].0.powi(2);

        let yi_2 = coords[row].1.powi(2);
        let ym_2 = coords[len-1].1.powi(2);

        let di_2 = rssi_to_distance(data[row].1.parse::<i32>().unwrap(), -60, 2).powi(2);
        let dm_2 = rssi_to_distance(data[len-1].1.parse::<i32>().unwrap(), -60, 2).powi(2);

        b[row] = xi_2 - xm_2 + yi_2 - ym_2 + di_2 - dm_2;

        //println!("xi_2: {}, xm_2: {}, yi_2: {}, ym_2: {}, di_2: {}, dm_2: {}",
        //xi_2, xm_2, yi_2, ym_2, di_2, dm_2);
        
        //println!("b[row]: {}", b[(row)]);
    }

    let x = (a.transpose() * a.clone()).try_inverse().unwrap() * a.transpose() * b;

    //println!("x[0]: {}, x[1]: {}", x[(0)], x[(1)]);
    
    let result = Locale(x[0], x[1], coords[0].2);

    Ok(result)
}



// dead zone
//
    // let test_data: RSSIData = vec![("node2".to_string(), "-40".to_string()),
    // ("node3".to_string(), "-10".to_string()),
    // ("node4".to_string(), "-60".to_string()),
    // ("node1".to_string(), "-10".to_string()),
    // ("node5".to_string(), "-80".to_string())]; 
/*    let value: RedisStream = cmd("XRANGE")
        .arg("COUNT").arg("17")
        .arg("STREAMS").arg(mac).arg("0")
        .query(con).unwrap();
*/  
//    println!("{}", mac);
//
//type RedisStream = Vec<Vec<(String, Vec<Vec<(String, Vec<(String, String)>)>>)>>;
