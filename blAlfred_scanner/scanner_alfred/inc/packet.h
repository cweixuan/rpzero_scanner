/* SPDX-License-Identifier: MIT */
/* Copyright (C) B.A.T.M.A.N. contributors:
 *
 * Simon Wunderlich, Sven Eckelmann
 *
 * License-Filename: LICENSES/preferred/MIT
 */

#ifndef _ALFRED_PACKET_H
#define _ALFRED_PACKET_H

#include <net/if.h>	/* IFNAMSIZ */

#define __packed __attribute__ ((packed))

/* basic blocks */

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
 * struct alfred_data - Data block header
 * @source: Mac address of the original source of the data
 * @header: TLV-header for the data
 * @data: "length" number of bytes followed by the header
 */
struct alfred_data {
	uint8_t source[ETH_ALEN];
	struct alfred_tlv header;
	/* flexible data block */
	__extension__ uint8_t data[0];
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

/**
 * enum alfred_packet_type - Types of packet stored in the main alfred_tlv
 * @ALFRED_PUSH_DATA: Packet is an alfred_push_data_v*
 * @ALFRED_ANNOUNCE_PRIMARY: Packet is an alfred_announce_primary_v*
 * @ALFRED_REQUEST: Packet is an alfred_request_v*
 * @ALFRED_STATUS_TXEND: Transaction was finished by sender
 * @ALFRED_STATUS_ERROR: Error was detected during the transaction
 * @ALFRED_MODESWITCH: Switch between different operation modes
 * @ALFRED_CHANGE_INTERFACE: Change the listening interface
 * @ALFRED_EVENT_REGISTER: Request to be notified about alfred update events
 * @ALFRED_EVENT_NOTIFY: Data record update has been received
 */
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

/* packets */

/**
 * struct alfred_push_data_v0 - Packet to push data blocks to another
 * @header: TLV header describing the complete packet
 * @tx: Transaction identificator and sequence number of packet
 * @data: multiple "alfred_data" blocks of arbitrary size (accumulated size
 *  stored in "header.length")
 *
 * alfred_push_data_v0 packets are always sent using unicast
 */
struct alfred_push_data_v0 {
	struct alfred_tlv header;
	struct alfred_transaction_mgmt tx;
	/* flexible data block */
	__extension__  struct alfred_data data[0];
} __packed;

/**
 * struct alfred_announce_primary_v0 - Hello packet sent by an alfred primary
 * @header: TLV header describing the complete packet
 *
 * Each alfred daemon running in primary mode sends it using multicast. The
 * receiver has to calculate the source using the network header
 */
struct alfred_announce_primary_v0 {
	struct alfred_tlv header;
} __packed;

/**
 * struct alfred_request_v0 - Request for a specific type
 * @header: TLV header describing the complete packet
 * @requested_type: data type which is requested
 * @tx_id: random identificator used for this transaction
 *
 * Sent as unicast to the node storing it
 */
struct alfred_request_v0 {
	struct alfred_tlv header;
	uint8_t requested_type;
	uint16_t tx_id;
} __packed;

/**
 * enum alfred_modeswitch_type - Mode of the daemon
 * @ALFRED_MODESWITCH_SECONDARY: see OPMODE_SECONDARY
 * @ALFRED_MODESWITCH_PRIMARY: see OPMODE_PRIMARY
 */
enum alfred_modeswitch_type {
	ALFRED_MODESWITCH_SECONDARY = 0,
	ALFRED_MODESWITCH_PRIMARY = 1,
};

/**
 * struct alfred_modeswitch_v0 - Request for a specific type
 * @header: TLV header describing the complete packet
 * @mode: data type which is requested
 *
 * Sent to the daemon by client
 */
struct alfred_modeswitch_v0 {
	struct alfred_tlv header;
	uint8_t mode;
} __packed;

/**
 * struct alfred_change_interface_v0 - Request to change the interface
 * @header: TLV header describing the complete packet
 * @ifaces: interface list (comma separated) to be changed to
 *
 * Sent to the daemon by client
 */
struct alfred_change_interface_v0 {
	struct alfred_tlv header;
	char ifaces[IFNAMSIZ * 16];
} __packed;

/**
 * struct alfred_change_bat_iface_v0 - Request to change the
 * batman-adv interface
 * @header: TLV header describing the complete packet
 * @bat_iface: interface to be changed to
 *
 * Sent to the daemon by client
 */
struct alfred_change_bat_iface_v0 {
	struct alfred_tlv header;
	char bat_iface[IFNAMSIZ];
};

/**
 * enum alfred_packet_type - Types of packet stored in the main alfred_tlv
 * @ALFRED_SERVER_MODE: Contains alfred mode information*
 * @ALFRED_SERVER_NET_IFACE: Contains alfred network interface information*
 * @ALFRED_SERVER_BAT_IFACE: Contains alfred batman interface information*
 */
enum alfred_server_status_type {
	ALFRED_SERVER_OP_MODE = 0,
	ALFRED_SERVER_NET_IFACE = 1,
	ALFRED_SERVER_BAT_IFACE = 2,
};

/**
 * struct alfred_server_status_req_v0 - server status request
 * @header: TLV header describing the complete packet
 *
 * Sent to the daemon by client
 */
struct alfred_server_status_req_v0 {
	struct alfred_tlv header;
} __packed;

/**
 * struct alfred_server_status_op_mode_v0 - server op mode status information
 * @op_mode: active op mode
 *
 * Sent to the client by daemon in response to status request
 */
struct alfred_server_status_op_mode_v0 {
	struct alfred_tlv header;
	uint8_t mode;
} __packed;

/**
 * struct alfred_server_status_net_iface_v0 - server net iface status information
 * @net_iface: configured network interface
 * @active: network interface active/inactive status info
 *
 * Sent to the client by daemon in response to status request
 */
struct alfred_server_status_net_iface_v0 {
	struct alfred_tlv header;
	char net_iface[IFNAMSIZ];
	uint8_t active;
} __packed;

/**
 * struct alfred_server_status_bat_iface_v0 - server bat iface status information
 * @op_mode: configured batman interface
 *
 * Sent to the client by daemon in response to status request
 */
struct alfred_server_status_bat_iface_v0 {
	struct alfred_tlv header;
	char bat_iface[IFNAMSIZ];
} __packed;

/**
 * struct alfred_server_status_rep_v0 - server status reply
 * @header: TLV header describing the complete packet
 *
 * Sent by the daemon to client in response to status request
 */
struct alfred_server_status_rep_v0 {
	struct alfred_tlv header;
} __packed;

/**
 * struct alfred_event_register_v0 - event registration message
 * @header: TLV header describing the complete packet
 *
 * Sent by the client to daemon to register for data record updates
 */
struct alfred_event_register_v0 {
	struct alfred_tlv header;
} __packed;

/**
 * struct alfred_event_notify_v0 - event notification message
 * @header: TLV header describing the complete packet
 * @type: Type of the data triggering the event
 * @source: Mac address of the original source of the data
 *
 * Sent by the daemon to client on data record update
 */
struct alfred_event_notify_v0 {
	struct alfred_tlv header;
	uint8_t type;
	uint8_t source[ETH_ALEN];
} __packed;

/**
 * struct alfred_status_v0 - Status info of a transaction
 * @header: TLV header describing the complete packet
 * @tx: Transaction identificator and sequence number of packet
 *
 * The sequence number has a special meaning. Failure status packets use
 * it to store the error code. Success status packets store the number of
 * transferred packets in it.
 *
 * Sent as unicast to the node requesting the data
 */
struct alfred_status_v0 {
	struct alfred_tlv header;
	struct alfred_transaction_mgmt tx;
} __packed;

#define ALFRED_VERSION			0
#define ALFRED_PORT			0x4242
#define ALFRED_MAX_RESERVED_TYPE	64
#define ALFRED_NUM_TYPES		256

#endif
