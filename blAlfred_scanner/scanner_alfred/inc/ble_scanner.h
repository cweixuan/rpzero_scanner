
#ifndef _BLE_SCANNER_H
#define _BLE_SCANNER_H


typedef struct bt_data_t {
    // char mac_addr[17];
    uint64_t mac_addr;
    int32_t rssi;
    time_t time;
}bt_data_t;

// int alfred_client_poll(struct globals *globals);


#endif
