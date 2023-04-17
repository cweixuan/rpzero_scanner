// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) B.A.T.M.A.N. contributors:
 *
 * Simon Wunderlich
 *
 * License-Filename: LICENSES/preferred/GPL-2.0
 */

#include <ctype.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <netinet/ether.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "client.h"
#include "alfred.h"


uint16_t get_random_id(void)
{
	return random();
}

int unix_sock_open_client(int* alfred_socket)
{
	struct sockaddr_un addr;
	const char* alfred_sockpath = ALFRED_SOCK_PATH_DEFAULT;
	*alfred_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (*alfred_socket < 0) {
		perror("can't create unix socket");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_LOCAL;
	strncpy(addr.sun_path, alfred_sockpath, sizeof(addr.sun_path));
	addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';

	if (connect(*alfred_socket, (struct sockaddr *)&addr,
		    sizeof(addr)) < 0) {
		close(*alfred_socket);
		perror("can't connect to unix socket");
		return -1;
	}

	return 0;
}

int unix_sock_close(int* alfred_socket)
{
	close(*alfred_socket);
	*alfred_socket = -1;
	return 0;
}



int alfred_send_data(int data_id,char* send_buf, int send_len)
{
	int alfred_socket;
	unsigned char buf[MAX_PAYLOAD];
	struct alfred_push_data_v0 *push;
	struct alfred_data *data;
	int ret, len;

	if (unix_sock_open_client(&alfred_socket))
		return -1;

	push = (struct alfred_push_data_v0 *)buf;
	data = push->data;
	len = sizeof(*push) + sizeof(*data);
	memcpy(&buf[len], send_buf, send_len);
	len+= send_len;

	//data ID
	push->header.type = ALFRED_PUSH_DATA;
	push->header.version = ALFRED_VERSION;
	push->header.length = htons(len - sizeof(push->header));
	push->tx.id = get_random_id();
	push->tx.seqno = htons(0);

	/* we leave data->source "empty" */
	memset(data->source, 0, sizeof(data->source));
	data->header.type = 65;
	data->header.version = 0;
	data->header.length = htons(len - sizeof(*push) - sizeof(*data));

	ret = write(alfred_socket, buf, len);
	if (ret != len)
		fprintf(stderr, "%s: only wrote %d of %d bytes: %s\n",
			__func__, ret, len, strerror(errno));
	unix_sock_close(&alfred_socket);
	return 0;
}

int alfred_req_data(int data_id, char* rx_buf, int *rx_len)
{
	
	int alfred_socket;
	unsigned char buf[MAX_PAYLOAD], *pos;
	struct alfred_request_v0 request;
	struct alfred_push_data_v0 *push;
	struct alfred_status_v0 *status;
	struct alfred_tlv *tlv;
	struct alfred_data *data;
	int ret, len, data_len, i;
	const size_t buf_data_len = sizeof(buf) - sizeof(*push) - sizeof(*data);

	if (unix_sock_open_client(&alfred_socket))
		return -1;

	len = sizeof(request);

	request.header.type = ALFRED_REQUEST;
	request.header.version = ALFRED_VERSION;
	request.header.length = FIXED_TLV_LEN(request);
	request.requested_type = data_id;
	request.tx_id = get_random_id();

	ret = write(alfred_socket, &request, len);
	if (ret != len)
		fprintf(stderr, "%s: only wrote %d of %d bytes: %s\n",
			__func__, ret, len, strerror(errno));

	push = (struct alfred_push_data_v0 *)buf;
	tlv = (struct alfred_tlv *)buf;
	while ((ret = read(alfred_socket, buf, sizeof(*tlv))) > 0) {
		if (ret < (int)sizeof(*tlv))
			break;

		if (tlv->type == ALFRED_STATUS_ERROR)
			goto recv_err;

		if (tlv->type != ALFRED_PUSH_DATA)
			break;

		/* read the rest of the header */
		ret = read(alfred_socket, buf + sizeof(*tlv),
			   sizeof(*push) - sizeof(*tlv));

		/* too short */
		if (ret < (int)(sizeof(*push) - (int)sizeof(*tlv)))
			break;

		/* read the rest of the header */
		ret = read(alfred_socket, buf + sizeof(*push),
			   sizeof(*data));

		if (ret < (ssize_t)sizeof(*data))
			break;

		data = push->data;
		data_len = ntohs(data->header.length);

		/* would it fit? it should! */
		if (data_len > (int)buf_data_len)
			break;

		/* read the data */
		ret = read(alfred_socket,
			   buf + sizeof(*push) + sizeof(*data), data_len);

		/* again too short */
		if (ret < data_len)
			break;

		pos = data->data;

		//ipv6 here
		// printf("{ \"%02x:%02x:%02x:%02x:%02x:%02x\", \"",
		//        data->source[0], data->source[1],
		//        data->source[2], data->source[3],
		//        data->source[4], data->source[5]);

		int max_len = *rx_len;
		*rx_len = 0;

		for (i = 0; i < data_len; i++) {
			if (*rx_len < max_len){
				rx_buf[*rx_len] = pos[i];
				*rx_len +=1;
			} else {
				break;
			}
		}
	}
	unix_sock_close(&alfred_socket);

	return 0;

recv_err:
	/* read the rest of the status message */
	ret = read(&alfred_socket, buf + sizeof(*tlv),
		   sizeof(*status) - sizeof(*tlv));

	/* too short */
	if (ret < (int)(sizeof(*status) - sizeof(*tlv)))
		return -1;

	status = (struct alfred_status_v0 *)buf;
	fprintf(stderr, "Request failed with %d\n", status->tx.seqno);

	return status->tx.seqno;
}


int alfred_send_ble_data(char* send_buf, int send_len){
	return alfred_send_data(65,send_buf,send_len);
}

int alfred_req_ble_data(char* rx_buf){
	return alfred_req_data(65,rx_buf);
}

