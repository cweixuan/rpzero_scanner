#include <iostream>
#include <unordered_map>
#include <vector>
#include <sw/redis++/redis++.h>
#include "alfredRX.h"
#include "client.h"

int main(){
    char rx_buf[MAX_PAYLOAD];
    int rx_len = 0;
    auto redis = sw::redis::Redis("tcp://127.0.0.1:6379");
    alfred_req_data_redis(65,redis);
    int curr_len = 0;
    int num_points;
    

    return 0;
}