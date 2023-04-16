#include "alfred.h"
#include <stdio.h>
#include "arraylist.h"

#define FILTER_LIST_LOCATION "./whitelist_macs.txt"

int alfred_client_poll(struct globals *globals){
    
    FILE *fp;
    
    fp = fopen(FILTER_LIST_LOCATION, "r");
    unsigned int addr[6];
    
    while (EOF != fscanf(fp,  "%2x:%2x:%2x:%2x:%2x:%2x\n", addr, addr+1,addr+2,addr+3,addr+4,addr+5))
    {
        printf("1 : %x:%x:%x:%x:%x:%x\n", addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
    }
    fclose(fp);
    return 0;

}