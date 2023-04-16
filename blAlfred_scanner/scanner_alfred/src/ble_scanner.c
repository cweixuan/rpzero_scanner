
#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <unistd.h>
#include "gattlib.h"
#include "arraylist.h"
#include "ble_scanner.h"
#include "deque.h"
#include "time.h"
#include "hashtable.h"
#include "client.h"

#define BLE_SCAN_TIMEOUT   3

typedef void (*ble_discovered_device_t)(const char* addr, const char* name);
Deque g_bt_data_deque;
// We use a mutex to make the BLE connections synchronous
static pthread_mutex_t bt_mutex = PTHREAD_MUTEX_INITIALIZER;
#define FILTER_LIST_LOCATION "./whitelist_macs.txt"


int read_mac_list(){
    //mutex here perhaps
	//todo: read in UUIDs not mac addresses zz
    arraylist *mac_filter_list = arraylist_create();
    FILE *fp;
    
    fp = fopen(FILTER_LIST_LOCATION, "r");
    unsigned int addr[6];
    
    while (EOF != fscanf(fp,  "%2x:%2x:%2x:%2x:%2x:%2x\n", addr+5, addr+4,addr+3,addr+2,addr+1,addr))
    {
        uint64_t mac_addr = (uint64_t)addr[5] << 40 | (uint64_t)addr[4] << 32 | 
		(uint64_t)addr[3] << 24 | (uint64_t)addr[2] << 16 | (uint64_t)addr[1] << 8 | (uint64_t)addr[0];
        arraylist_add(mac_filter_list, &mac_addr);
    }
    fclose(fp);
    return 0;

}

int read_uuid_list(){
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

void bt_data_pack(bt_data_t *dest, uint64_t mac_addr,int RSSI, time_t curr_time ){
	dest->mac_addr = mac_addr;
	dest->rssi = RSSI;
	dest->time = curr_time;
	return;
}

static void ble_discovered_device(void *adapter, const char* addr, const char* name, void *user_data) {
	// struct connection_t *connection;
	// struct bt_globals *bt_globals = user_data;
	int ret;
	int16_t rssi = 0;
	ret =	gattlib_get_rssi_from_mac(adapter, addr, &rssi);

	unsigned int haddr[6];
	// printf("Discovered %s | RSSI: %d\n", addr,rssi);
    sscanf(addr,  "%2x:%2x:%2x:%2x:%2x:%2x", haddr+5, haddr+4,haddr+3,haddr+2,haddr+1,haddr);
	uint64_t mac_addr = (uint64_t)haddr[5] << 40 | (uint64_t)haddr[4] << 32 | 
	(uint64_t)haddr[3] << 24 | (uint64_t)haddr[2] << 16 | (uint64_t)haddr[1] << 8 | (uint64_t)haddr[0];
	bt_data_t *temp = malloc(sizeof(bt_data_t));
	if (temp == NULL){
		printf("failed to allocate new bt_data node\n");
		return;
	}
	// printf("Discovered %6lx | RSSI: %d\n", mac_addr,rssi);
	bt_data_pack(temp, mac_addr, rssi, time(NULL));
	deque_append(g_bt_data_deque, temp);

}

int bt_thread_func(struct bt_globals *bt_globals) {
	const char* adapter_name;
	void* adapter;
	int ret;
	adapter_name = NULL;
	printf("hello\n");
	uint64_t i = 0;
	while (1){
		printf("scan %ld\r\n",i++);
		ret = gattlib_adapter_open(adapter_name, &adapter);
		if (ret) {
			//insert logic for failing to open adapter, reset the program? crash?
			GATTLIB_LOG(GATTLIB_ERROR, "Failed to open adapter.");
			return 1;
		}
		pthread_mutex_lock(&bt_mutex);
		ret = gattlib_adapter_scan_enable(adapter, ble_discovered_device, BLE_SCAN_TIMEOUT, bt_globals /* user_data */);
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


int data_update_thread_func(struct bt_globals *bt_globals){
	//setup deque for data
	g_bt_data_deque = deque_create(NULL);
	if (g_bt_data_deque == NULL){
		printf("Data Deque failed to init\n");
		return -1;
	}

	//setup hashtable and keylist for keeping track of each beacon
	arraylist* key_list = arraylist_create();
	
	HashTable* data_table = malloc(sizeof(HashTable));
	if (data_table == NULL){
		printf("Data hashtable failed to init\n");
		return -1;
	}
	ht_setup(data_table, sizeof(uint64_t), sizeof(bt_data_t), 20);
	//inser tstuff lol
	while (1){
		if (g_bt_data_deque != NULL){
		pthread_mutex_lock(&bt_mutex);
			while (deque_count(g_bt_data_deque)){
				bt_data_t *temp = deque_pop(g_bt_data_deque);
				if (temp != NULL){
					//memory was allocated elsewhere so need to free
					if (!ht_contains(data_table,&temp->mac_addr)){
						// add into arraylist for iteration
						uint64_t* malloc_key = malloc(sizeof(uint64_t));
						if (malloc_key != NULL){
							*malloc_key = temp->mac_addr;
							arraylist_add(key_list,malloc_key);
						}
					}
					ht_insert(data_table,&temp->mac_addr, temp);
					free(temp);
				}
			}
		pthread_mutex_unlock(&bt_mutex);
		uint64_t* curr_addr;
		time_t current_time = time(NULL);
		char tx_buf[MAX_PAYLOAD];
		int tx_len = 0;
		for (int i =0; i < key_list->size; i ++){
			curr_addr = arraylist_get(key_list,i);
			if (curr_addr != NULL){
				bt_data_t *data_temp;
				data_temp = ht_lookup(data_table, curr_addr);
				if (data_temp != NULL){
					//check if last datapoint was > 1h ago
					if ( current_time - data_temp->time > 3600 ){
						arraylist_remove(key_list,i);
						ht_erase(data_table,curr_addr);
						free(curr_addr);
					} else {
						printf("Discovered %6lx | RSSI: %d at Time: %s", 
						data_temp->mac_addr,data_temp->rssi, ctime(&data_temp->time));
						tx_len += sprintf(tx_buf+tx_len,"Discovered %6lx | RSSI: %d at Time: %s \n||", 
						data_temp->mac_addr,data_temp->rssi, ctime(&data_temp->time));
					}
				}
			}
		}
		alfred_send_data(tx_buf, tx_len);
		}
		sleep(10);
	}
}


int main(int argc, char argv[]){
	 pthread_t pt_bt_thread;
	 pthread_t test_thread;
	void *ret;

	if (pthread_create(&pt_bt_thread, NULL, bt_thread_func, "bt thread") != 0) {
		perror("pthread_create() error");
		exit(1);
	}
	
	if (pthread_create(&test_thread, NULL, data_update_thread_func, "bt thread") != 0) {
		perror("pthread_create() error");
		exit(1);
	}


	if (pthread_join(pt_bt_thread, &ret) != 0) {
		perror("pthread_create() error");
		exit(3);
	}	
	if (pthread_join(test_thread, &ret) != 0) {
		perror("pthread_create() error");
		exit(3);
	}
  printf("thread exited with '%s'\n", ret);
}