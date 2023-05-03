#ifndef __ALFREDRX_H
#define __ALFREDRX_H
typedef struct bt_data_t {
    // char mac_addr[17];
    uint64_t mac_addr;
    int64_t rssi;
    time_t time;
}bt_data_t;

typedef struct bt_packed_data_t{
    uint64_t mac_addr;
    uint64_t time;
    int64_t rssi;

} bt_packed_data_t __attribute__ ((packed));

#endif