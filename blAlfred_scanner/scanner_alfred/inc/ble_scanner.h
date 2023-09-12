
#ifndef _BLE_SCANNER_H
#define _BLE_SCANNER_H


typedef struct bt_data_t {
    // char mac_addr[17];
    uint64_t mac_addr;
    int32_t rssi;
    time_t time;
}bt_data_t;

typedef struct bt_packed_data_t{
    uint64_t mac_addr;
    uint64_t time;
    int64_t rssi;

} bt_packed_data_t __attribute__ ((packed));

// int alfred_client_poll(struct globals *globals);


#endif
