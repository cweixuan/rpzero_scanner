use redis::{Client, cmd, Connection};
use std::fs; 

type RedisStream = Vec<Vec<(String, Vec<Vec<(String, Vec<(String, String)>)>>)>>;

fn main() {
    let redis_key: &str = "node_coords";
    let file_path: &str = "/home/jasshank/work/coord_map/coords.txt";

    println!("Loading from: {}", file_path);

    let client = Client::open("redis://127.0.0.1/").expect("Redis server connection failed!");
    let mut con: Connection = client.get_connection().unwrap();

    let content = fs::read_to_string(file_path).expect("File read failed!");

    load_redis(&mut con, redis_key, content);
}

fn load_redis(con: &mut Connection,redis_key: &str, content: String) {
    let _value: i32 = cmd("DEL")
        .arg(redis_key)
        .query(con).unwrap();

    let lines = content.split("\n");

    for line in lines {
        let line_split = line.split(" ").collect::<Vec<&str>>();

        if line_split.len() >= 2 {
            println!("{}", line);
            let node_name: &str = line_split[0];
            let coords: &str = line_split[1];

            let _value: i32 = cmd("HSET")
                .arg(redis_key)
                .arg(node_name).arg(coords)
                .query(con).unwrap();
        }
    }
}
