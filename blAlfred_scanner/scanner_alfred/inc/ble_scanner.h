
#ifndef _BLE_SCANNER_H
#define _BLE_SCANNER_H


typedef struct bt_data_t {
    // char mac_addr[17];
    uint64_t mac_addr;
    int rssi;
    time_t time;
    void* next;
    void* prev;
}bt_data_t;

typedef struct{
    bt_data_t* list_head;
    bt_data_t* list_tail;
    int size;
}bt_data_list_t;



struct bt_globals {
    bt_data_list_t* bluetooth_data_list;
};
// int alfred_client_poll(struct globals *globals);


#endif
