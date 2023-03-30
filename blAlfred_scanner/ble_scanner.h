
#ifndef _BLE_SCANNER_H
#define _BLE_SCANNER_H



typedef struct{
    bt_data_t* list_head;
    bt_data_t* list_tail;
    int size;
}bt_data_list_t;

typedef struct {
    char mac_addr[17];
    int RSSI;
    bt_data_t* next;
    bt_data_t* prev;
}bt_data_t;

struct bt_globals {
    bt_data_list_t* bluetooth_data_list;
};
int alfred_client_poll(struct globals *globals);


#endif
