#include "alfred.h"
#include <stdio.h>
#include "arraylist.h"


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
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

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



static void ble_discovered_device(void *adapter, const char* addr, const char* name, void *user_data) {
	struct connection_t *connection;
	int ret;

	int16_t rssi = 0;
	ret =	gattlib_get_rssi_from_mac(adapter, addr, &rssi);
	if (rssi == 0){
		ret =	gattlib_get_rssi_from_mac(adapter, addr, &rssi);
	}
	if (rssi == 0){
		ret =	gattlib_get_rssi_from_mac(adapter, addr, &rssi);
	}
	printf("ret:%d\n", ret);
	if (name) {
		printf("Discovered %s - '%s' | RSSI: %d\n", addr, name, rssi);
	} else {
		printf("Discovered %s | RSSI: %d\n", addr,rssi);
	}
}

int main(int argc, const char *argv[]) {
	const char* adapter_name;
	void* adapter;
	int ret;
	int num_tries = 9999;

	if (argc == 1) {
		adapter_name = NULL;
	} else if (argc == 2) {
		sscanf(argv[1], "%d", &num_tries);
		printf("Scanning for %d times",num_tries);
	} else if (argc == 3) {
		sscanf(argv[1], "%d", &num_tries);
		printf("Scanning for %d times",num_tries);
		adapter_name = argv[2];
	} else
	{
		printf("%s [<bluetooth-adapter>]\n", argv[0]);
		return 1;
	}

#ifdef GATTLIB_LOG_BACKEND_SYSLOG
	openlog("gattlib_ble_scan", LOG_CONS | LOG_NDELAY | LOG_PERROR, LOG_USER);
	setlogmask(LOG_UPTO(LOG_INFO));
#endif
	LIST_INIT(&g_ble_connections);

	for (uint8_t i = 1; i <= num_tries; i ++){
		printf("scan %d\n",i);
		ret = gattlib_adapter_open(adapter_name, &adapter);
		if (ret) {
			GATTLIB_LOG(GATTLIB_ERROR, "Failed to open adapter.");
			return 1;
		}
		pthread_mutex_lock(&g_mutex);
		ret = gattlib_adapter_scan_enable(adapter, ble_discovered_device, BLE_SCAN_TIMEOUT, NULL /* user_data */);
		if (ret) {
			GATTLIB_LOG(GATTLIB_ERROR, "Failed to scan.");
			goto EXIT;
		}
		//sleep(5);
		gattlib_adapter_scan_disable(adapter);
		//remove the MACs so that we can get another RSSI value
		pthread_mutex_unlock(&g_mutex);
		gattlib_adapter_close(adapter);
	}
	puts("Scan completed");

EXIT:
	gattlib_adapter_close(adapter);
	return ret;
}
