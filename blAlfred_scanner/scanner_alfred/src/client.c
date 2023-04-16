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
	return 0;
}

int alfred_send_data(char* send_buf, int send_len)
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