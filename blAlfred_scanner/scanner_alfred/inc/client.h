#ifndef __CLIENT_H
#define __CLIENT_H

#include <net/if.h>	/* IFNAMSIZ */
#include <net/ethernet.h>
#include <netinet/udp.h>
#include <sys/types.h>

#define MAX_PAYLOAD ((1 << 16) - 1 - sizeof(struct udphdr))
uint16_t get_random_id(void);
int unix_sock_open_client(int* alfred_socket);
int unix_sock_close(int* alfred_socket);
int alfred_send_data(char* send_buf, int send_len);

#endif