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
#include <iomanip>
#include <string_view>

#include <netinet/ether.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <sw/redis++/redis++.h>
#include <iostream>
#include "client.h"
#include "alfred.h"
#include "alfredRX.h"


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

template< typename T >
std::string int_to_hex( T i )
{
  std::stringstream stream;
  stream << std::setw(12) << std::setfill('0')  << std::hex << i;
  return stream.str();
}

int alfred_req_data_redis(int data_id, sw::redis::Redis &redis)
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

	std::unordered_map<uint64_t,std::vector<std::pair<std::string,int64_t>>> data_map;
	
	while ((ret = read(alfred_socket, buf, sizeof(*tlv))) > 0) {
		if (ret < (int)sizeof(*tlv))
			break;

		if (tlv->type == ALFRED_STATUS_ERROR)
			// goto recv_err;
			return -1;

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
		uint64_t src_macaddr_uint = (uint64_t)data->source[0] << 40 | (uint64_t)data->source[1] << 32 | 
			(uint64_t)data->source[2] << 24 | (uint64_t)data->source[3] << 16 | 
			(uint64_t)data->source[4] << 8 | (uint64_t)data->source[5];
			std::string src_macaddr = int_to_hex<uint64_t>(src_macaddr_uint);
		//rxbuf temp processing
		bt_packed_data_t temp_packed;
		uint16_t num_vals = 0;
		memcpy(&num_vals, pos, sizeof(uint16_t));
		uint32_t curr_len = 2;
		printf("Number of MACs detected: %d\n",num_vals);
		printf("%ld | %d\n", sizeof(bt_packed_data_t), data_len);
		for (uint32_t i =0; i < num_vals; i ++){
			if(data_len  - curr_len < sizeof(bt_packed_data_t)) {
				printf("why is there not enough data here >:()\n");
				break;
			}
			memcpy(&temp_packed, pos+curr_len, sizeof(bt_packed_data_t));
			curr_len += sizeof(bt_packed_data_t);
			printf("Discovered %10lx | RSSI: %ld | at time %ld\n",
			temp_packed.mac_addr, temp_packed.rssi,temp_packed.time);
			std::pair<std::string,int64_t> temp_pair(src_macaddr, temp_packed.rssi);
			if (data_map.count(temp_packed.mac_addr) > 0){
				data_map.at(temp_packed.mac_addr).emplace_back(temp_pair);
			} else {
				std::vector<std::pair<std::string,int64_t>> temp_vector;
				temp_vector.emplace_back(temp_pair);
				data_map.insert({temp_packed.mac_addr,temp_vector});
			}
		}
	}
	unix_sock_close(&alfred_socket);
	for (std::pair<uint64_t,std::vector<std::pair<std::string, int64_t>>> curr_mac : data_map){
		auto mac_hex = int_to_hex<uint64_t>(std::get<0>(curr_mac));
		std::cout << mac_hex << "\n";
		auto id =redis.xadd(mac_hex, "*",std::get<1>(curr_mac).begin(), std::get<1>(curr_mac).end());
		std::get<1>(curr_mac).clear();
	}

	return 0;

// recv_err:
// 	/* read the rest of the status message */
// 	ret = read(alfred_socket, buf + sizeof(*tlv),
// 		   sizeof(*status) - sizeof(*tlv));

// 	/* too short */
// 	if (ret < (int)(sizeof(*status) - sizeof(*tlv)))
// 		return -1;

// 	status = (struct alfred_status_v0 *)buf;
// 	fprintf(stderr, "Request failed with %d\n", status->tx.seqno);

// 	return status->tx.seqno;
}

