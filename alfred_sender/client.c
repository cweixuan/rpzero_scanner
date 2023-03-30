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
#include "alfred.h"
#include "packet.h"
#include <sys/time.h>

int alfred_client_request_data(struct globals *globals)
{
	
	unsigned char buf[MAX_PAYLOAD], *pos;
	struct alfred_request_v0 request;
	struct alfred_push_data_v0 *push;
	struct alfred_status_v0 *status;
	struct alfred_tlv *tlv;
	struct alfred_data *data;
	int ret, len, data_len, i;
	const size_t buf_data_len = sizeof(buf) - sizeof(*push) - sizeof(*data);

	if (unix_sock_open_client(globals))
		return -1;

	len = sizeof(request);

	request.header.type = ALFRED_REQUEST;
	request.header.version = ALFRED_VERSION;
	request.header.length = FIXED_TLV_LEN(request);
	request.requested_type = globals->clientmode_arg;
	request.tx_id = get_random_id();

	ret = write(globals->unix_sock, &request, len);
	if (ret != len)
		fprintf(stderr, "%s: only wrote %d of %d bytes: %s\n",
			__func__, ret, len, strerror(errno));

	push = (struct alfred_push_data_v0 *)buf;
	tlv = (struct alfred_tlv *)buf;
	while ((ret = read(globals->unix_sock, buf, sizeof(*tlv))) > 0) {
		if (ret < (int)sizeof(*tlv))
			break;

		if (tlv->type == ALFRED_STATUS_ERROR)
			goto recv_err;

		if (tlv->type != ALFRED_PUSH_DATA)
			break;

		/* read the rest of the header */
		ret = read(globals->unix_sock, buf + sizeof(*tlv),
			   sizeof(*push) - sizeof(*tlv));

		/* too short */
		if (ret < (int)(sizeof(*push) - (int)sizeof(*tlv)))
			break;

		/* read the rest of the header */
		ret = read(globals->unix_sock, buf + sizeof(*push),
			   sizeof(*data));

		if (ret < (ssize_t)sizeof(*data))
			break;

		data = push->data;
		data_len = ntohs(data->header.length);

		/* would it fit? it should! */
		if (data_len > (int)buf_data_len)
			break;

		/* read the data */
		ret = read(globals->unix_sock,
			   buf + sizeof(*push) + sizeof(*data), data_len);

		/* again too short */
		if (ret < data_len)
			break;

		pos = data->data;


		//ipv6 here
		printf("{ \"%02x:%02x:%02x:%02x:%02x:%02x\", \"",
		       data->source[0], data->source[1],
		       data->source[2], data->source[3],
		       data->source[4], data->source[5]);


		for (i = 0; i < data_len; i++) {
			if (pos[i] == '"')
				printf("\\\"");
			else if (pos[i] == '\\')
				printf("\\\\");
			else if (!isprint(pos[i]))
				printf("\\x%02x", pos[i]);
			else
				printf("%c", pos[i]);
		}

		printf("\"");

		if (globals->verbose)
			printf(", %u", data->header.version);

		printf(" hello here's where it gets the data (it's in data->source)},\n");
	}
	printf("line 120 in client.c\n");
	unix_sock_close(globals);

	return 0;

recv_err:
	/* read the rest of the status message */
	ret = read(globals->unix_sock, buf + sizeof(*tlv),
		   sizeof(*status) - sizeof(*tlv));

	/* too short */
	if (ret < (int)(sizeof(*status) - sizeof(*tlv)))
		return -1;

	status = (struct alfred_status_v0 *)buf;
	fprintf(stderr, "Request failed with %d\n", status->tx.seqno);

	return status->tx.seqno;
}

int alfred_client_set_data(struct globals *globals)
{
	unsigned char buf[MAX_PAYLOAD];
	struct alfred_push_data_v0 *push;
	struct alfred_data *data;
	int ret, len;

	if (unix_sock_open_client(globals))
		return -1;

	push = (struct alfred_push_data_v0 *)buf;
	data = push->data;
	len = sizeof(*push) + sizeof(*data);
	while (!feof(stdin)) {
		ret = fread(&buf[len], 1, sizeof(buf) - len, stdin);
		len += ret;

		if (sizeof(buf) == len)
			break;
	}

	push->header.type = ALFRED_PUSH_DATA;
	push->header.version = ALFRED_VERSION;
	push->header.length = htons(len - sizeof(push->header));
	push->tx.id = get_random_id();
	push->tx.seqno = htons(0);

	/* we leave data->source "empty" */
	memset(data->source, 0, sizeof(data->source));
	data->header.type = globals->clientmode_arg;
	data->header.version = globals->clientmode_version;
	data->header.length = htons(len - sizeof(*push) - sizeof(*data));

	ret = write(globals->unix_sock, buf, len);
	if (ret != len)
		fprintf(stderr, "%s: only wrote %d of %d bytes: %s\n",
			__func__, ret, len, strerror(errno));

	unix_sock_close(globals);
	return 0;
}


int alfred_client_send_char(struct globals *globals)
{
	unsigned char buf[MAX_PAYLOAD];
	const char test[] = "test_send";
	struct alfred_push_data_v0 *push;
	struct alfred_data *data;
	int ret, len;

	if (unix_sock_open_client(globals))
		return -1;

	push = (struct alfred_push_data_v0 *)buf;
	data = push->data;
	len = sizeof(*push) + sizeof(*data);
	memcpy(&buf[len], test, sizeof(test));
	len+= sizeof(test);

	push->header.type = ALFRED_PUSH_DATA;
	push->header.version = ALFRED_VERSION;
	push->header.length = htons(len - sizeof(push->header));
	push->tx.id = get_random_id();
	push->tx.seqno = htons(0);

	/* we leave data->source "empty" */
	memset(data->source, 0, sizeof(data->source));
	data->header.type = globals->clientmode_arg;
	data->header.version = globals->clientmode_version;
	data->header.length = htons(len - sizeof(*push) - sizeof(*data));

	ret = write(globals->unix_sock, buf, len);
	if (ret != len)
		fprintf(stderr, "%s: only wrote %d of %d bytes: %s\n",
			__func__, ret, len, strerror(errno));

	unix_sock_close(globals);
	return 0;
}

int alfred_client_modeswitch(struct globals *globals)
{
	struct alfred_modeswitch_v0 modeswitch;
	int ret, len;

	if (unix_sock_open_client(globals))
		return -1;

	len = sizeof(modeswitch);

	modeswitch.header.type = ALFRED_MODESWITCH;
	modeswitch.header.version = ALFRED_VERSION;
	modeswitch.header.length = FIXED_TLV_LEN(modeswitch);

	switch (globals->opmode) {
	case OPMODE_SECONDARY:
		modeswitch.mode = ALFRED_MODESWITCH_SECONDARY;
		break;
	case OPMODE_PRIMARY:
		modeswitch.mode = ALFRED_MODESWITCH_PRIMARY;
		break;
	default:
		fprintf(stderr, "%s: unknown opmode %u in modeswitch\n",
			__func__, globals->opmode);
		return -1;
	}

	ret = write(globals->unix_sock, &modeswitch, len);
	if (ret != len)
		fprintf(stderr, "%s: only wrote %d of %d bytes: %s\n",
			__func__, ret, len, strerror(errno));

	unix_sock_close(globals);
	return 0;
}


int alfred_client_server_status(struct globals *globals)
{
	struct alfred_server_status_net_iface_v0 *status_net_iface;
	struct alfred_server_status_bat_iface_v0 *status_bat_iface;
	struct alfred_server_status_op_mode_v0 *status_op_mode;
	struct alfred_server_status_req_v0 status_req;
	struct alfred_server_status_rep_v0 *status_rep;
	int ret, tlvsize, headsize, len, consumed;
	struct alfred_tlv *status_tlv;
	uint8_t buf[MAX_PAYLOAD];

	if (unix_sock_open_client(globals))
		return -1;

	len = sizeof(status_req);
	memset(&status_req, 0, len);

	status_req.header.type = ALFRED_SERVER_STATUS;
	status_req.header.version = ALFRED_VERSION;
	status_req.header.length = 0;

	ret = write(globals->unix_sock, (unsigned char *)&status_req, len);
	if (ret != len)
		fprintf(stderr, "%s: only wrote %d of %d bytes: %s\n",
			__func__, ret, len, strerror(errno));

	len = read(globals->unix_sock, buf, sizeof(buf));
	if (len <= 0) {
		perror("read from unix socket failed");
		goto err;
	}

	ret = -1;
	status_rep = (struct alfred_server_status_rep_v0 *)buf;

	/* drop too small packets */
	headsize = sizeof(status_rep->header);
	if (len < headsize) {
		perror("unexpected header size received from unix socket");
		goto err;
	}

	if ((len - headsize) < ((int)ntohs(status_rep->header.length))) {
		perror("unexpected packet size received from unix socket");
		goto err;
	}

	if (status_rep->header.type != ALFRED_SERVER_STATUS) {
		perror("alfred server_status type mismatch");
		goto err;
	}

	if (status_rep->header.version != ALFRED_VERSION) {
		perror("alfred version mismatch");
		goto err;
	}

	headsize = ntohs(status_rep->header.length);

	if (headsize < (int)(sizeof(*status_rep) - sizeof(status_rep->header)))
		goto err;

	consumed = sizeof(*status_rep);

	while (len - consumed > 0) {
		if (len - consumed < (int)sizeof(*status_tlv))
			break;

		status_tlv = (struct alfred_tlv *)(buf + consumed);

		if (status_tlv->version != ALFRED_VERSION)
			break;

		tlvsize = ntohs(status_tlv->length);
		tlvsize += sizeof(*status_tlv);

		if (len - consumed < tlvsize)
			break;

		switch (status_tlv->type) {
		case ALFRED_SERVER_OP_MODE:
			if (tlvsize != sizeof(*status_op_mode))
				break;

			status_op_mode = (struct alfred_server_status_op_mode_v0 *)(buf + consumed);

			switch (status_op_mode->mode) {
			case ALFRED_MODESWITCH_SECONDARY:
				printf("- mode: secondary\n");
				break;
			case ALFRED_MODESWITCH_PRIMARY:
				printf("- mode: primary\n");
				break;
			default:
				printf("- mode: unknown\n");
				break;
			}

			break;
		case ALFRED_SERVER_NET_IFACE:
			if (tlvsize != sizeof(*status_net_iface))
				break;

			status_net_iface = (struct alfred_server_status_net_iface_v0 *)(buf + consumed);
			status_net_iface->net_iface[sizeof(status_net_iface->net_iface) - 1] = 0;
			printf("- interface: %s\n", status_net_iface->net_iface);
			printf("\t- status: %s\n",
				status_net_iface->active == 1 ? "active" : "inactive");
			break;
		case ALFRED_SERVER_BAT_IFACE:
			if (tlvsize != sizeof(*status_bat_iface))
				break;

			status_bat_iface = (struct alfred_server_status_bat_iface_v0 *)(buf + consumed);
			status_bat_iface->bat_iface[sizeof(status_bat_iface->bat_iface) - 1] = 0;
			printf("- batman-adv interface: %s\n", status_bat_iface->bat_iface);
			break;
		}

		consumed += tlvsize;
	}

err:
	unix_sock_close(globals);
	return 0;
}


int alfred_client_event_monitor(struct globals *globals)
{
	struct alfred_event_register_v0 event_register;
	struct alfred_event_notify_v0 event_notify;
	int ret, len;

	if (unix_sock_open_client(globals))
		return -1;

	len = sizeof(event_register);

	event_register.header.type = ALFRED_EVENT_REGISTER;
	event_register.header.version = ALFRED_VERSION;
	event_register.header.length = 0;

	ret = write(globals->unix_sock, &event_register, len);
	if (ret != len) {
		fprintf(stderr, "%s: only wrote %d of %d bytes: %s\n",
			__func__, ret, len, strerror(errno));
		goto err;
	}

	while (true) {
		len = read(globals->unix_sock, &event_notify, sizeof(event_notify));
		if (len == 0) {
			fprintf(stdout, "Server closed the connection\n");
			goto err;
		}

		if (len < 0) {
			perror("read from unix socket failed");
			goto err;
		}

		if (len != sizeof(event_notify)) {
			fprintf(stderr, "notify read bytes: %d (expected: %zu)\n",
				len, sizeof(event_notify));
				goto err;
		}

		if (event_notify.header.version != ALFRED_VERSION)
			continue;

		if (event_notify.header.type != ALFRED_EVENT_NOTIFY)
			continue;

		fprintf(stdout, "Event: type = %hhu, source = %02x:%02x:%02x:%02x:%02x:%02x\n",
			event_notify.type,
			event_notify.source[0], event_notify.source[1],
			event_notify.source[2], event_notify.source[3],
			event_notify.source[4], event_notify.source[5]);
	}

err:
	unix_sock_close(globals);
	return 0;
}



int alfred_client_stress(struct globals *globals)
{
	char test_buffer[500];
	char rx_buffer[500];
	time_t start_time = time(0);
	time_t avg_time = 0;
	struct timeval tval_before, tval_after, tval_result;
	gettimeofday(&tval_before, NULL);
	for(uint64_t i =0; i < 0xFFFFFFFFFFFFFF ; i ++){
	int max_rx = 500;
		int filled_chars = sprintf(test_buffer, "TEST %ld", i);
		alfred_client_cust_send(globals,test_buffer,filled_chars);
		alfred_client_cust_req(globals,rx_buffer,&max_rx);
		//compare the rx and sent data
		avg_time += time(0)-start_time;
		uint8_t test_rx = 0;
		for (int j = 0; j < filled_chars; j ++){
			if (test_buffer[j] != rx_buffer[j]){
				test_rx = 1;
				break;
			}
		}
		if (test_rx == 1){
			printf("Tried to send %.*s\nReceived %.*s instead\n", 
			filled_chars, test_buffer, max_rx, rx_buffer);
			printf("TX failed at %ld at time %s\n", 
			i, asctime(localtime(&globals->prev_sent_time)));
		}
		if (i % 1000 == 0){
			gettimeofday(&tval_after, NULL);
			timersub(&tval_after, &tval_before, &tval_result);
			long int time_taken = (long int)tval_result.tv_usec/1000;
			long int bytes_per_sec = filled_chars * 1000000 / time_taken;
			printf("Time elapsed: %06ldus\n",  time_taken);
			printf("Est datarate: %06ldkbps\n", bytes_per_sec);
			printf("Currently sending  %.*s\n"
			,filled_chars,test_buffer);
			avg_time=0;
			gettimeofday(&tval_before, NULL);
		}
	}
	return 0;
}


int alfred_client_cust_send(struct globals *globals, char* send_buf, int send_len)
{
	unsigned char buf[MAX_PAYLOAD];
	struct alfred_push_data_v0 *push;
	struct alfred_data *data;
	int ret, len;

	if (unix_sock_open_client(globals))
		return -1;

	push = (struct alfred_push_data_v0 *)buf;
	data = push->data;
	len = sizeof(*push) + sizeof(*data);
	memcpy(&buf[len], send_buf, send_len);
	len+= send_len;

	push->header.type = ALFRED_PUSH_DATA;
	push->header.version = ALFRED_VERSION;
	push->header.length = htons(len - sizeof(push->header));
	push->tx.id = get_random_id();
	push->tx.seqno = htons(0);

	/* we leave data->source "empty" */
	memset(data->source, 0, sizeof(data->source));
	data->header.type = globals->clientmode_arg;
	data->header.version = globals->clientmode_version;
	data->header.length = htons(len - sizeof(*push) - sizeof(*data));

	ret = write(globals->unix_sock, buf, len);
	if (ret != len)
		fprintf(stderr, "%s: only wrote %d of %d bytes: %s\n",
			__func__, ret, len, strerror(errno));
	globals->prev_sent_time = time(0);
	unix_sock_close(globals);
	return 0;
}


int alfred_client_cust_req(struct globals *globals, char* rx_buf, int *rx_len)
{
	unsigned char buf[MAX_PAYLOAD], *pos;
	struct alfred_request_v0 request;
	struct alfred_push_data_v0 *push;
	struct alfred_status_v0 *status;
	struct alfred_tlv *tlv;
	struct alfred_data *data;
	int ret, len, data_len, i;
	const size_t buf_data_len = sizeof(buf) - sizeof(*push) - sizeof(*data);

	if (unix_sock_open_client(globals))
		return -1;

	len = sizeof(request);

	request.header.type = ALFRED_REQUEST;
	request.header.version = ALFRED_VERSION;
	request.header.length = FIXED_TLV_LEN(request);
	request.requested_type = globals->clientmode_arg;
	request.tx_id = get_random_id();

	ret = write(globals->unix_sock, &request, len);
	if (ret != len)
		fprintf(stderr, "%s: only wrote %d of %d bytes: %s\n",
			__func__, ret, len, strerror(errno));

	push = (struct alfred_push_data_v0 *)buf;
	tlv = (struct alfred_tlv *)buf;
	while ((ret = read(globals->unix_sock, buf, sizeof(*tlv))) > 0) {
		if (ret < (int)sizeof(*tlv))
			break;

		if (tlv->type == ALFRED_STATUS_ERROR)
			goto recv_err;

		if (tlv->type != ALFRED_PUSH_DATA)
			break;

		/* read the rest of the header */
		ret = read(globals->unix_sock, buf + sizeof(*tlv),
			   sizeof(*push) - sizeof(*tlv));

		/* too short */
		if (ret < (int)(sizeof(*push) - (int)sizeof(*tlv)))
			break;

		/* read the rest of the header */
		ret = read(globals->unix_sock, buf + sizeof(*push),
			   sizeof(*data));

		if (ret < (ssize_t)sizeof(*data))
			break;

		data = push->data;
		data_len = ntohs(data->header.length);

		/* would it fit? it should! */
		if (data_len > (int)buf_data_len)
			break;

		/* read the data */
		ret = read(globals->unix_sock,
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

		if (globals->verbose)
			printf(", %u", data->header.version);

	}
	unix_sock_close(globals);

	return 0;

recv_err:
	/* read the rest of the status message */
	ret = read(globals->unix_sock, buf + sizeof(*tlv),
		   sizeof(*status) - sizeof(*tlv));

	/* too short */
	if (ret < (int)(sizeof(*status) - sizeof(*tlv)))
		return -1;

	status = (struct alfred_status_v0 *)buf;
	fprintf(stderr, "Request failed with %d\n", status->tx.seqno);

	return status->tx.seqno;
}
