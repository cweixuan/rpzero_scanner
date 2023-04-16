/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) B.A.T.M.A.N. contributors:
 *
 * Simon Wunderlich
 *
 * License-Filename: LICENSES/preferred/GPL-2.0
 */
#ifndef __ALFRED_H
#define __ALFRED_H

#ifndef SOURCE_VERSION
#define SOURCE_VERSION			"2023.1"
#endif

#define ALFRED_SOCK_PATH_DEFAULT	"/var/run/alfred.sock"
#define __packed __attribute__ ((packed))
#ifndef ETH_ALEN
#define ETH_ALEN	6		/* from <net/ethernet.h> */
#endif

enum alfred_packet_type {
	ALFRED_PUSH_DATA = 0,
	ALFRED_ANNOUNCE_PRIMARY = 1,
	ALFRED_REQUEST = 2,
	ALFRED_STATUS_TXEND = 3,
	ALFRED_STATUS_ERROR = 4,
	ALFRED_MODESWITCH = 5,
	ALFRED_CHANGE_INTERFACE = 6,
	ALFRED_CHANGE_BAT_IFACE = 7,
	ALFRED_SERVER_STATUS = 8,
	ALFRED_EVENT_REGISTER = 9,
	ALFRED_EVENT_NOTIFY = 10,
};

enum clientmode {
	CLIENT_NONE,
	CLIENT_REQUEST_DATA,
	CLIENT_SET_DATA,
	CLIENT_MODESWITCH,
	CLIENT_CHANGE_INTERFACE,
	CLIENT_CHANGE_BAT_IFACE,
	CLIENT_SERVER_STATUS,
	CLIENT_EVENT_MONITOR,
	CLIENT_SEND_CHAR,
	CLIENT_STRESS_TEST
};

/**
 * struct alfred_tlv - Type (Version) Length part of a TLV
 * @type: Type of the data
 * @version: Version of the data
 * @length: Length of the data without the alfred_tlv header
 */
struct alfred_tlv {
	uint8_t type;
	uint8_t version;
	uint16_t length;
} __packed;

/**
 * struct alfred_transaction_mgmt - Transaction Mgmt block for multiple packets
 * @id: random identificator used for this transaction
 * @seqno: Number of packet inside a transaction
 */
struct alfred_transaction_mgmt {
	uint16_t id;
	uint16_t seqno;
} __packed;

struct alfred_data {
	uint8_t source[ETH_ALEN];
	struct alfred_tlv header;
	/* flexible data block */
	__extension__ uint8_t data[0];
} __packed;

struct alfred_push_data_v0 {
	struct alfred_tlv header;
	struct alfred_transaction_mgmt tx;
	/* flexible data block */
	__extension__  struct alfred_data data[0];
} __packed;

#define ALFRED_VERSION			0
#define ALFRED_PORT			0x4242
#define ALFRED_MAX_RESERVED_TYPE	64
#define ALFRED_NUM_TYPES		256


#endif