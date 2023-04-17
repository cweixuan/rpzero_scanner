#include <iostream>
#include <unordered_map>
#include <vector>
#include "alfredRX.h"
#include "client.h"

int main(){
    char rx_buf[MAX_PAYLOAD];
    int rx_len = 0;
    std::unordered_map<uint64_t, std::vector<bt_data_t>> test;
    alfred_req_ble_data(rx_buf, &rx_len);
    int curr_len = 0;
    int num_points;

    return 0;
}