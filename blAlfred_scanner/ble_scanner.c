#include "alfred.h"
#include <stdio.h>
#include "arraylist.h"

#include "ble_scanner.h"
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <unistd.h>
#include "gattlib.h"

#define BLE_SCAN_TIMEOUT   4

typedef void (*ble_discovered_device_t)(const char* addr, const char* name);

// We use a mutex to make the BLE connections synchronous
static pthread_mutex_t bt_mutex = PTHREAD_MUTEX_INITIALIZER;

LIST_HEAD(listhead, connection_t) g_ble_connections;
struct connection_t {
	pthread_t thread;
	char* addr;
	LIST_ENTRY(connection_t) entries;
};
#define FILTER_LIST_LOCATION "./whitelist_macs.txt"

//non packed version for local storage
typedef struct {
    uint64_t mac_addr;
    uint8_t RSSI;

}scan_data_t;

//total 8 bytes
typedef struct __packed{
    uint64_t mac_addr:48;
    uint8_t RSSI;
    uint8_t padding[1];

}scan_send_data_t;

typedef struct __packed {
    uint16_t len;
    scan_scan_data_t *data;

} scan_packet_t;


int read_mac_list(struct globals *globals){
    //mutex here perhaps
	//todo: read in UUIDs not mac addresses zz
    arraylist *mac_filter_list = arraylist_create();
    FILE *fp;
    
    fp = fopen(FILTER_LIST_LOCATION, "r");
    unsigned int addr[6];
    
    while (EOF != fscanf(fp,  "%2x:%2x:%2x:%2x:%2x:%2x\n", addr+5, addr+4,addr+3,addr+2,addr+1,addr))
    {
        uint64_t mac_addr = addr[5] << 40 | addr[4] << 32 | addr[3] << 24 | addr[2] << 16 | addr[1] << 8 | addr[0];
        arraylist_add(mac_filter_list, mac_addr);
    }
    fclose(fp);
    return 0;

}

int read_uuid_list(struct globals *globals){
    // //mutex here perhaps
	// //todo: read in UUIDs not mac addresses zz
    // arraylist *mac_filter_list = arraylist_create();
    // FILE *fp;
    
    // fp = fopen(FILTER_LIST_LOCATION, "r");
    // unsigned int addr[6];
    
    // while (EOF != fscanf(fp,  "%2x:%2x:%2x:%2x:%2x:%2x\n", addr+5, addr+4,addr+3,addr+2,addr+1,addr))
    // {
    //     uint64_t mac_addr = addr[5] << 40 | addr[4] << 32 | addr[3] << 24 | addr[2] << 16 | addr[1] << 8 | addr[0];
    //     arraylist_add(mac_filter_list, mac_addr);
    // }
    // fclose(fp);
    return 0;

}

//stack with linked list implemenetation
//todo: check if malloc returns null to prevent seg faults
int push_bt_data(bt_data_t data, bt_data_list_t* data_list){
	if (data_list == NULL){
		//list has not been initialised, data will now be head
		bt_data_t* temp = malloc(sizeof(bt_data_t));
		data_list = malloc(sizeof(bt_data_list_t));
		*temp = data;
		data_list->head = temp;
		data_list->tail = temp;
		data_list->size = 1;
		return 0;
	} else {
		//insert after the tail
		bt_data_t* temp = malloc(sizeof(bt_data_t));
		*temp = data;
		data_list->tail.next = temp;
		temp->prev = data_list->tail;
		data_list->tail = temp;
		data_list->size += 1;
		return 0;
	}
}

int pop_bt_data(bt_data_t *data, bt_data_list_t* data_list){
	if (data_list == NULL){
		//list not initialised...
		return 1;
	} else {
		//copy tail data into *data
		*data = data_list->list_tail;
		
		//FREE THE MEMORY
		free(data_list->list_tail);

		//shift tail back by 1
		data_list->tail = data.prev;
		data_list->size -=1;
		return 0;
	}
}



static void ble_discovered_device(void *adapter, const char* addr, const char* name, void *user_data) {
	struct connection_t *connection;
	struct bt_globals *bt_globals = user_data;
	int ret;

	int16_t rssi = 0;
	ret =	gattlib_get_rssi_from_mac(adapter, addr, &rssi);

	bt_data_t bt_data;
	bt_data.mac_addr
	bt_globals->bluetooth_data_list


	printf("ret:%d\n", ret);
	if (name) {
		printf("Discovered %s - '%s' | RSSI: %d\n", addr, name, rssi);
	} else {
		printf("Discovered %s | RSSI: %d\n", addr,rssi);
	}
}

int bt_thread(struct bt_globals *bt_globals) {
	const char* adapter_name;
	void* adapter;
	int ret;
	adapter_name = NULL;

#ifdef GATTLIB_LOG_BACKEND_SYSLOG
	openlog("gattlib_ble_scan", LOG_CONS | LOG_NDELAY | LOG_PERROR, LOG_USER);
	setlogmask(LOG_UPTO(LOG_INFO));
#endif
	LIST_INIT(&g_ble_connections);

	while (1){
		printf("scan %d\n",i);
		ret = gattlib_adapter_open(adapter_name, &adapter);
		if (ret) {
			//insert logic for failing to open adapter, reset the program? crash?
			GATTLIB_LOG(GATTLIB_ERROR, "Failed to open adapter.");
			return 1;
		}
		pthread_mutex_lock(&bt_mutex);
		ret = gattlib_adapter_scan_enable_with_filter(adapter, ble_discovered_device, BLE_SCAN_TIMEOUT, bt_globals /* user_data */);
		if (ret) {
			GATTLIB_LOG(GATTLIB_ERROR, "Failed to scan.");
			goto EXIT;
		}
		gattlib_adapter_scan_disable(adapter);
		//remove the MACs so that we can get another RSSI value
		pthread_mutex_unlock(&bt_mutex);
		gattlib_adapter_close(adapter);
	}
	puts("Scan completed");

EXIT:
	gattlib_adapter_close(adapter);
	return ret;
}


int data_update_thread(){
	//inser tstuff lol
}

int main(int argc, char argv[]){
	//stuff
}
