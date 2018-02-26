/*
 * Copyright (c) 2004, 2005 Topspin Communications.
 * Copyright (c) 2004, 2011-2012 Intel Corporation.
 * Copyright (c) 2005, 2006, 2007 Cisco Systems, Inc.
 * Copyright (c) 2005, PathScale, Inc.
 * Copyright (c) 2018, Annika Wierichs, RWTH Aachen.
 * All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses. You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * - Redistributions of source code must retain the above
 * copyright notice, this list of conditions and the following
 * disclaimer.
 *
 * - Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials
 * provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * This header file contains all necessary verbs resource definitions and API
 * function declarations. Functions are defined in ibv.c.
 *
 * Comment blocks from the original header have been removed, refer to the
 * original verbs.h:
 * https://github.com/linux-rdma/rdma-core/blob/master/libibverbs/verbs.h
 *
 * Current API Version: 1.1.6
 */

#ifndef __HERMIT_VERBS_H__
#define __HERMIT_VERBS_H__

#ifdef __KERNEL__
#include <hermit/stddef.h>
#include <hermit/errno.h>
#include <hermit/string.h>
#else
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// TODO: Move the following definitions to appropriate header files.

// Definitions from linux/types.h and in-l64.h (left out __bitwise):
typedef unsigned short __u16;
typedef unsigned int   __u32;
typedef unsigned long  __u64;
typedef __u16 __be16;
typedef __u32 __be32;
typedef __u64 __be64;

#ifdef __KERNEL__
// Definitions from pthread.h:
typedef struct _pthread_descr_struct *_pthread_descr;
typedef long long int __pthread_cond_align_t;

struct _pthread_fastlock {
	long int __status;
	int __spinlock;
};

typedef struct {
	int __m_reserved;
	int __m_count;
	_pthread_descr __m_owner;
	int __m_kind;
	struct _pthread_fastlock __m_lock;
} pthread_mutex_t;

typedef struct {
	struct _pthread_fastlock __c_lock;
	_pthread_descr __c_waiting;
	char __padding[48 - sizeof(struct _pthread_fastlock) -
		sizeof(_pthread_descr) -
		sizeof(__pthread_cond_align_t)];
	__pthread_cond_align_t __align;
} pthread_cond_t;

// Definitions from time.h:
typedef long __kernel_time_t;

struct timespec {
	__kernel_time_t tv_sec; /* seconds */
	long tv_nsec; /* nanoseconds */
};
#endif // __KERNEL__


/*
 * Infiniband resource definitions
 */

union ibv_gid {
	uint8_t raw[16];
	struct {
		__be64 subnet_prefix;
		__be64 interface_id;
	} global;
};

enum ibv_node_type {
	IBV_NODE_UNKNOWN = -1,
	IBV_NODE_CA = 1,
	IBV_NODE_SWITCH,
	IBV_NODE_ROUTER,
	IBV_NODE_RNIC,
	IBV_NODE_USNIC,
	IBV_NODE_USNIC_UDP,
};

enum ibv_transport_type {
	IBV_TRANSPORT_UNKNOWN = -1,
	IBV_TRANSPORT_IB = 0,
	IBV_TRANSPORT_IWARP,
	IBV_TRANSPORT_USNIC,
	IBV_TRANSPORT_USNIC_UDP,
};

enum ibv_device_cap_flags {
	IBV_DEVICE_RESIZE_MAX_WR = 1,
	IBV_DEVICE_BAD_PKEY_CNTR = 1 << 1,
	IBV_DEVICE_BAD_QKEY_CNTR = 1 << 2,
	IBV_DEVICE_RAW_MULTI = 1 << 3,
	IBV_DEVICE_AUTO_PATH_MIG = 1 << 4,
	IBV_DEVICE_CHANGE_PHY_PORT = 1 << 5,
	IBV_DEVICE_UD_AV_PORT_ENFORCE = 1 << 6,
	IBV_DEVICE_CURR_QP_STATE_MOD = 1 << 7,
	IBV_DEVICE_SHUTDOWN_PORT = 1 << 8,
	IBV_DEVICE_INIT_TYPE = 1 << 9,
	IBV_DEVICE_PORT_ACTIVE_EVENT = 1 << 10,
	IBV_DEVICE_SYS_IMAGE_GUID = 1 << 11,
	IBV_DEVICE_RC_RNR_NAK_GEN = 1 << 12,
	IBV_DEVICE_SRQ_RESIZE = 1 << 13,
	IBV_DEVICE_N_NOTIFY_CQ = 1 << 14,
	IBV_DEVICE_MEM_WINDOW = 1 << 17,
	IBV_DEVICE_UD_IP_CSUM = 1 << 18,
	IBV_DEVICE_XRC = 1 << 20,
	IBV_DEVICE_MEM_MGT_EXTENSIONS = 1 << 21,
	IBV_DEVICE_MEM_WINDOW_TYPE_2A = 1 << 23,
	IBV_DEVICE_MEM_WINDOW_TYPE_2B = 1 << 24,
	IBV_DEVICE_RC_IP_CSUM = 1 << 25,
	IBV_DEVICE_RAW_IP_CSUM = 1 << 26,
	IBV_DEVICE_MANAGED_FLOW_STEERING = 1 << 29
};

enum ibv_atomic_cap {
	IBV_ATOMIC_NONE,
	IBV_ATOMIC_HCA,
	IBV_ATOMIC_GLOB
};

struct ibv_device_attr {
	char fw_ver[64];
	__be64 node_guid;
	__be64 sys_image_guid;
	uint64_t max_mr_size;
	uint64_t page_size_cap;
	uint32_t vendor_id;
	uint32_t vendor_part_id;
	uint32_t hw_ver;
	int max_qp;
	int max_qp_wr;
	int device_cap_flags;
	int max_sge;
	int max_sge_rd;
	int max_cq;
	int max_cqe;
	int max_mr;
	int max_pd;
	int max_qp_rd_atom;
	int max_ee_rd_atom;
	int max_res_rd_atom;
	int max_qp_init_rd_atom;
	int max_ee_init_rd_atom;
	enum ibv_atomic_cap atomic_cap;
	int max_ee;
	int max_rdd;
	int max_mw;
	int max_raw_ipv6_qp;
	int max_raw_ethy_qp;
	int max_mcast_grp;
	int max_mcast_qp_attach;
	int max_total_mcast_qp_attach;
	int max_ah;
	int max_fmr;
	int max_map_per_fmr;
	int max_srq;
	int max_srq_wr;
	int max_srq_sge;
	uint16_t max_pkeys;
	uint8_t local_ca_ack_delay;
	uint8_t phys_port_cnt;
};

struct ibv_query_device_ex_input {
	uint32_t comp_mask;
};

enum ibv_odp_transport_cap_bits {
	IBV_ODP_SUPPORT_SEND = 1 << 0,
	IBV_ODP_SUPPORT_RECV = 1 << 1,
	IBV_ODP_SUPPORT_WRITE = 1 << 2,
	IBV_ODP_SUPPORT_READ = 1 << 3,
	IBV_ODP_SUPPORT_ATOMIC = 1 << 4,
};

struct ibv_odp_caps {
	uint64_t general_caps;
	struct {
		uint32_t rc_odp_caps;
		uint32_t uc_odp_caps;
		uint32_t ud_odp_caps;
	} per_transport_caps;
};

enum ibv_odp_general_caps {
	IBV_ODP_SUPPORT = 1 << 0,
};

struct ibv_tso_caps {
	uint32_t max_tso;
	uint32_t supported_qpts;
};

enum ibv_rx_hash_function_flags {
	IBV_RX_HASH_FUNC_TOEPLITZ = 1 << 0,
};

enum ibv_rx_hash_fields {
	IBV_RX_HASH_SRC_IPV4 = 1 << 0,
	IBV_RX_HASH_DST_IPV4 = 1 << 1,
	IBV_RX_HASH_SRC_IPV6 = 1 << 2,
	IBV_RX_HASH_DST_IPV6 = 1 << 3,
	IBV_RX_HASH_SRC_PORT_TCP = 1 << 4,
	IBV_RX_HASH_DST_PORT_TCP = 1 << 5,
	IBV_RX_HASH_SRC_PORT_UDP = 1 << 6,
	IBV_RX_HASH_DST_PORT_UDP = 1 << 7
};

struct ibv_rss_caps {
	uint32_t supported_qpts;
	uint32_t max_rwq_indirection_tables;
	uint32_t max_rwq_indirection_table_size;
	uint64_t rx_hash_fields_mask; /* enum ibv_rx_hash_fields */
	uint8_t rx_hash_function; /* enum ibv_rx_hash_function_flags */
};

struct ibv_packet_pacing_caps {
	uint32_t qp_rate_limit_min;
	uint32_t qp_rate_limit_max; /* In kbps */
	uint32_t supported_qpts;
};

enum ibv_raw_packet_caps {
	IBV_RAW_PACKET_CAP_CVLAN_STRIPPING = 1 << 0,
	IBV_RAW_PACKET_CAP_SCATTER_FCS = 1 << 1,
	IBV_RAW_PACKET_CAP_IP_CSUM = 1 << 2,
	IBV_RAW_PACKET_CAP_DELAY_DROP = 1 << 3,
};

struct ibv_device_attr_ex {
	struct ibv_device_attr orig_attr;
	uint32_t comp_mask;
	struct ibv_odp_caps odp_caps;
	uint64_t completion_timestamp_mask;
	uint64_t hca_core_clock;
	uint64_t device_cap_flags_ex;
	struct ibv_tso_caps tso_caps;
	struct ibv_rss_caps rss_caps;
	uint32_t max_wq_type_rq;
	struct ibv_packet_pacing_caps packet_pacing_caps;
	uint32_t raw_packet_caps; /* Use ibv_raw_packet_caps */
};

enum ibv_mtu {
	IBV_MTU_256 = 1,
	IBV_MTU_512 = 2,
	IBV_MTU_1024 = 3,
	IBV_MTU_2048 = 4,
	IBV_MTU_4096 = 5
};

enum ibv_port_state {
	IBV_PORT_NOP = 0,
	IBV_PORT_DOWN = 1,
	IBV_PORT_INIT = 2,
	IBV_PORT_ARMED = 3,
	IBV_PORT_ACTIVE = 4,
	IBV_PORT_ACTIVE_DEFER = 5
};

enum {
	IBV_LINK_LAYER_UNSPECIFIED,
	IBV_LINK_LAYER_INFINIBAND,
	IBV_LINK_LAYER_ETHERNET,
};

enum ibv_port_cap_flags {
	IBV_PORT_SM = 1 << 1,
	IBV_PORT_NOTICE_SUP = 1 << 2,
	IBV_PORT_TRAP_SUP = 1 << 3,
	IBV_PORT_OPT_IPD_SUP = 1 << 4,
	IBV_PORT_AUTO_MIGR_SUP = 1 << 5,
	IBV_PORT_SL_MAP_SUP = 1 << 6,
	IBV_PORT_MKEY_NVRAM = 1 << 7,
	IBV_PORT_PKEY_NVRAM = 1 << 8,
	IBV_PORT_LED_INFO_SUP = 1 << 9,
	IBV_PORT_SYS_IMAGE_GUID_SUP = 1 << 11,
	IBV_PORT_PKEY_SW_EXT_PORT_TRAP_SUP = 1 << 12,
	IBV_PORT_EXTENDED_SPEEDS_SUP = 1 << 14,
	IBV_PORT_CM_SUP = 1 << 16,
	IBV_PORT_SNMP_TUNNEL_SUP = 1 << 17,
	IBV_PORT_REINIT_SUP = 1 << 18,
	IBV_PORT_DEVICE_MGMT_SUP = 1 << 19,
	IBV_PORT_VENDOR_CLASS_SUP = 1 << 20,
	IBV_PORT_DR_NOTICE_SUP = 1 << 21,
	IBV_PORT_CAP_MASK_NOTICE_SUP = 1 << 22,
	IBV_PORT_BOOT_MGMT_SUP = 1 << 23,
	IBV_PORT_LINK_LATENCY_SUP = 1 << 24,
	IBV_PORT_CLIENT_REG_SUP = 1 << 25,
	IBV_PORT_IP_BASED_GIDS = 1 << 26
};

struct ibv_port_attr {
	enum ibv_port_state state;
	enum ibv_mtu max_mtu;
	enum ibv_mtu active_mtu;
	int gid_tbl_len;
	uint32_t port_cap_flags;
	uint32_t max_msg_sz;
	uint32_t bad_pkey_cntr;
	uint32_t qkey_viol_cntr;
	uint16_t pkey_tbl_len;
	uint16_t lid;
	uint16_t sm_lid;
	uint8_t lmc;
	uint8_t max_vl_num;
	uint8_t sm_sl;
	uint8_t subnet_timeout;
	uint8_t init_type_reply;
	uint8_t active_width;
	uint8_t active_speed;
	uint8_t phys_state;
	uint8_t link_layer;
	uint8_t reserved;
};

enum ibv_event_type {
	IBV_EVENT_CQ_ERR,
	IBV_EVENT_QP_FATAL,
	IBV_EVENT_QP_REQ_ERR,
	IBV_EVENT_QP_ACCESS_ERR,
	IBV_EVENT_COMM_EST,
	IBV_EVENT_SQ_DRAINED,
	IBV_EVENT_PATH_MIG,
	IBV_EVENT_PATH_MIG_ERR,
	IBV_EVENT_DEVICE_FATAL,
	IBV_EVENT_PORT_ACTIVE,
	IBV_EVENT_PORT_ERR,
	IBV_EVENT_LID_CHANGE,
	IBV_EVENT_PKEY_CHANGE,
	IBV_EVENT_SM_CHANGE,
	IBV_EVENT_SRQ_ERR,
	IBV_EVENT_SRQ_LIMIT_REACHED,
	IBV_EVENT_QP_LAST_WQE_REACHED,
	IBV_EVENT_CLIENT_REREGISTER,
	IBV_EVENT_GID_CHANGE,
	IBV_EVENT_WQ_FATAL,
};

struct ibv_async_event {
	union {
		struct ibv_cq *cq;
		struct ibv_qp *qp;
		struct ibv_srq *srq;
		struct ibv_wq *wq;
		int port_num;
	} element;
	enum ibv_event_type event_type;
};

enum ibv_wc_status {
	IBV_WC_SUCCESS,
	IBV_WC_LOC_LEN_ERR,
	IBV_WC_LOC_QP_OP_ERR,
	IBV_WC_LOC_EEC_OP_ERR,
	IBV_WC_LOC_PROT_ERR,
	IBV_WC_WR_FLUSH_ERR,
	IBV_WC_MW_BIND_ERR,
	IBV_WC_BAD_RESP_ERR,
	IBV_WC_LOC_ACCESS_ERR,
	IBV_WC_REM_INV_REQ_ERR,
	IBV_WC_REM_ACCESS_ERR,
	IBV_WC_REM_OP_ERR,
	IBV_WC_RETRY_EXC_ERR,
	IBV_WC_RNR_RETRY_EXC_ERR,
	IBV_WC_LOC_RDD_VIOL_ERR,
	IBV_WC_REM_INV_RD_REQ_ERR,
	IBV_WC_REM_ABORT_ERR,
	IBV_WC_INV_EECN_ERR,
	IBV_WC_INV_EEC_STATE_ERR,
	IBV_WC_FATAL_ERR,
	IBV_WC_RESP_TIMEOUT_ERR,
	IBV_WC_GENERAL_ERR,
	IBV_WC_TM_ERR,
	IBV_WC_TM_RNDV_INCOMPLETE,
};

enum ibv_wc_opcode {
	IBV_WC_SEND,
	IBV_WC_RDMA_WRITE,
	IBV_WC_RDMA_READ,
	IBV_WC_COMP_SWAP,
	IBV_WC_FETCH_ADD,
	IBV_WC_BIND_MW,
	IBV_WC_LOCAL_INV,
	IBV_WC_TSO,
/*
 * Set value of IBV_WC_RECV so consumers can test if a completion is a
 * receive by testing (opcode & IBV_WC_RECV).
 */
	IBV_WC_RECV = 1 << 7,
	IBV_WC_RECV_RDMA_WITH_IMM
};

enum {
	IBV_WC_IP_CSUM_OK_SHIFT = 2
};

enum ibv_create_cq_wc_flags {
	IBV_WC_EX_WITH_BYTE_LEN = 1 << 0,
	IBV_WC_EX_WITH_IMM = 1 << 1,
	IBV_WC_EX_WITH_QP_NUM = 1 << 2,
	IBV_WC_EX_WITH_SRC_QP = 1 << 3,
	IBV_WC_EX_WITH_SLID = 1 << 4,
	IBV_WC_EX_WITH_SL = 1 << 5,
	IBV_WC_EX_WITH_DLID_PATH_BITS = 1 << 6,
	IBV_WC_EX_WITH_COMPLETION_TIMESTAMP = 1 << 7,
	IBV_WC_EX_WITH_CVLAN = 1 << 8,
	IBV_WC_EX_WITH_FLOW_TAG = 1 << 9,
};

enum {
	IBV_WC_STANDARD_FLAGS = IBV_WC_EX_WITH_BYTE_LEN |
				 IBV_WC_EX_WITH_IMM |
				 IBV_WC_EX_WITH_QP_NUM |
				 IBV_WC_EX_WITH_SRC_QP |
				 IBV_WC_EX_WITH_SLID |
				 IBV_WC_EX_WITH_SL |
				 IBV_WC_EX_WITH_DLID_PATH_BITS
};

enum {
	IBV_CREATE_CQ_SUP_WC_FLAGS = IBV_WC_STANDARD_FLAGS |
				IBV_WC_EX_WITH_COMPLETION_TIMESTAMP |
				IBV_WC_EX_WITH_CVLAN |
				IBV_WC_EX_WITH_FLOW_TAG
};

enum ibv_wc_flags {
	IBV_WC_GRH = 1 << 0,
	IBV_WC_WITH_IMM = 1 << 1,
	IBV_WC_IP_CSUM_OK = 1 << IBV_WC_IP_CSUM_OK_SHIFT,
	IBV_WC_WITH_INV = 1 << 3
};

struct ibv_wc {
	uint64_t wr_id;
	enum ibv_wc_status status;
	enum ibv_wc_opcode opcode;
	uint32_t vendor_err;
	uint32_t byte_len;
	/* When (wc_flags & IBV_WC_WITH_IMM): Immediate data in network byte order.
	 * When (wc_flags & IBV_WC_WITH_INV): Stores the invalidated rkey.
	 */
	union {
		__be32 imm_data;
		uint32_t invalidated_rkey;
	};
	uint32_t qp_num;
	uint32_t src_qp;
	int wc_flags;
	uint16_t pkey_index;
	uint16_t slid;
	uint8_t sl;
	uint8_t dlid_path_bits;
};

enum ibv_access_flags {
	IBV_ACCESS_LOCAL_WRITE = 1,
	IBV_ACCESS_REMOTE_WRITE = (1<<1),
	IBV_ACCESS_REMOTE_READ = (1<<2),
	IBV_ACCESS_REMOTE_ATOMIC = (1<<3),
	IBV_ACCESS_MW_BIND = (1<<4),
	IBV_ACCESS_ZERO_BASED = (1<<5),
	IBV_ACCESS_ON_DEMAND = (1<<6),
};

struct ibv_mw_bind_info {
	struct ibv_mr *mr;
	uint64_t addr;
	uint64_t length;
	int mw_access_flags; /* use ibv_access_flags */
};

struct ibv_pd {
	struct ibv_context *context;
	uint32_t handle;
};

enum ibv_xrcd_init_attr_mask {
	IBV_XRCD_INIT_ATTR_FD = 1 << 0,
	IBV_XRCD_INIT_ATTR_OFLAGS = 1 << 1,
	IBV_XRCD_INIT_ATTR_RESERVED = 1 << 2
};

struct ibv_xrcd_init_attr {
	uint32_t comp_mask;
	int fd;
	int oflags;
};

struct ibv_xrcd {
	struct ibv_context *context;
};

enum ibv_rereg_mr_flags {
	IBV_REREG_MR_CHANGE_TRANSLATION = (1 << 0),
	IBV_REREG_MR_CHANGE_PD = (1 << 1),
	IBV_REREG_MR_CHANGE_ACCESS = (1 << 2),
	IBV_REREG_MR_KEEP_VALID = (1 << 3),
	IBV_REREG_MR_FLAGS_SUPPORTED = ((IBV_REREG_MR_KEEP_VALID << 1) - 1)
};

struct ibv_mr {
	struct ibv_context *context;
	struct ibv_pd *pd;
	void *addr;
	size_t length;
	uint32_t handle;
	uint32_t lkey;
	uint32_t rkey;
};

enum ibv_mw_type {
	IBV_MW_TYPE_1 = 1,
	IBV_MW_TYPE_2 = 2
};

struct ibv_mw {
	struct ibv_context *context;
	struct ibv_pd *pd;
	uint32_t rkey;
	uint32_t handle;
	enum ibv_mw_type type;
};

struct ibv_global_route {
	union ibv_gid dgid;
	uint32_t flow_label;
	uint8_t sgid_index;
	uint8_t hop_limit;
	uint8_t traffic_class;
};

struct ibv_grh {
	__be32 version_tclass_flow;
	__be16 paylen;
	uint8_t next_hdr;
	uint8_t hop_limit;
	union ibv_gid sgid;
	union ibv_gid dgid;
};

enum ibv_rate {
	IBV_RATE_MAX = 0,
	IBV_RATE_2_5_GBPS = 2,
	IBV_RATE_5_GBPS = 5,
	IBV_RATE_10_GBPS = 3,
	IBV_RATE_20_GBPS = 6,
	IBV_RATE_30_GBPS = 4,
	IBV_RATE_40_GBPS = 7,
	IBV_RATE_60_GBPS = 8,
	IBV_RATE_80_GBPS = 9,
	IBV_RATE_120_GBPS = 10,
	IBV_RATE_14_GBPS = 11,
	IBV_RATE_56_GBPS = 12,
	IBV_RATE_112_GBPS = 13,
	IBV_RATE_168_GBPS = 14,
	IBV_RATE_25_GBPS = 15,
	IBV_RATE_100_GBPS = 16,
	IBV_RATE_200_GBPS = 17,
	IBV_RATE_300_GBPS = 18
};

struct ibv_ah_attr {
	struct ibv_global_route grh;
	uint16_t dlid;
	uint8_t sl;
	uint8_t src_path_bits;
	uint8_t static_rate;
	uint8_t is_global;
	uint8_t port_num;
};

enum ibv_srq_attr_mask {
	IBV_SRQ_MAX_WR = 1 << 0,
	IBV_SRQ_LIMIT = 1 << 1
};

struct ibv_srq_attr {
	uint32_t max_wr;
	uint32_t max_sge;
	uint32_t srq_limit;
};

struct ibv_srq_init_attr {
	void *srq_context;
	struct ibv_srq_attr attr;
};

enum ibv_srq_type {
	IBV_SRQT_BASIC,
	IBV_SRQT_XRC
};

enum ibv_srq_init_attr_mask {
	IBV_SRQ_INIT_ATTR_TYPE = 1 << 0,
	IBV_SRQ_INIT_ATTR_PD = 1 << 1,
	IBV_SRQ_INIT_ATTR_XRCD = 1 << 2,
	IBV_SRQ_INIT_ATTR_CQ = 1 << 3,
	IBV_SRQ_INIT_ATTR_RESERVED = 1 << 4
};

struct ibv_srq_init_attr_ex {
	void *srq_context;
	struct ibv_srq_attr attr;

	uint32_t comp_mask;
	enum ibv_srq_type srq_type;
	struct ibv_pd *pd;
	struct ibv_xrcd *xrcd;
	struct ibv_cq *cq;
};

enum ibv_wq_type {
	IBV_WQT_RQ
};

enum ibv_wq_init_attr_mask {
	IBV_WQ_INIT_ATTR_FLAGS = 1 << 0,
	IBV_WQ_INIT_ATTR_RESERVED = 1 << 1,
};

enum ibv_wq_flags {
	IBV_WQ_FLAGS_CVLAN_STRIPPING = 1 << 0,
	IBV_WQ_FLAGS_SCATTER_FCS = 1 << 1,
	IBV_WQ_FLAGS_DELAY_DROP = 1 << 2,
	IBV_WQ_FLAGS_RESERVED = 1 << 3,
};

struct ibv_wq_init_attr {
	void *wq_context;
	enum ibv_wq_type wq_type;
	uint32_t max_wr;
	uint32_t max_sge;
	struct ibv_pd *pd;
	struct ibv_cq *cq;
	uint32_t comp_mask; /* Use ibv_wq_init_attr_mask */
	uint32_t create_flags; /* use ibv_wq_flags */
};

enum ibv_wq_state {
	IBV_WQS_RESET,
	IBV_WQS_RDY,
	IBV_WQS_ERR,
	IBV_WQS_UNKNOWN
};

enum ibv_wq_attr_mask {
	IBV_WQ_ATTR_STATE = 1 << 0,
	IBV_WQ_ATTR_CURR_STATE = 1 << 1,
	IBV_WQ_ATTR_FLAGS = 1 << 2,
	IBV_WQ_ATTR_RESERVED = 1 << 3,
};

struct ibv_wq_attr {
	uint32_t attr_mask;
	enum ibv_wq_state wq_state;
	enum ibv_wq_state curr_wq_state;
	uint32_t flags; /* Use ibv_wq_flags */
	uint32_t flags_mask; /* Use ibv_wq_flags */
};

struct ibv_rwq_ind_table {
	struct ibv_context *context;
	int ind_tbl_handle;
	int ind_tbl_num;
	uint32_t comp_mask;
};

enum ibv_ind_table_init_attr_mask {
	IBV_CREATE_IND_TABLE_RESERVED = (1 << 0)
};

struct ibv_rwq_ind_table_init_attr {
	uint32_t log_ind_tbl_size;
	struct ibv_wq **ind_tbl; // Each entry is a pointer to a Receive Work Queue
	uint32_t comp_mask;
};

enum ibv_qp_type {
	IBV_QPT_RC = 2,
	IBV_QPT_UC,
	IBV_QPT_UD,
	IBV_QPT_RAW_PACKET = 8,
	IBV_QPT_XRC_SEND = 9,
	IBV_QPT_XRC_RECV
};

struct ibv_qp_cap {
	uint32_t max_send_wr;
	uint32_t max_recv_wr;
	uint32_t max_send_sge;
	uint32_t max_recv_sge;
	uint32_t max_inline_data;
};

struct ibv_qp_init_attr {
	void *qp_context;
	struct ibv_cq *send_cq;
	struct ibv_cq *recv_cq;
	struct ibv_srq *srq;
	struct ibv_qp_cap cap;
	enum ibv_qp_type qp_type;
	int sq_sig_all;
};

enum ibv_qp_init_attr_mask {
	IBV_QP_INIT_ATTR_PD = 1 << 0,
	IBV_QP_INIT_ATTR_XRCD = 1 << 1,
	IBV_QP_INIT_ATTR_CREATE_FLAGS = 1 << 2,
	IBV_QP_INIT_ATTR_MAX_TSO_HEADER = 1 << 3,
	IBV_QP_INIT_ATTR_IND_TABLE = 1 << 4,
	IBV_QP_INIT_ATTR_RX_HASH = 1 << 5,
	IBV_QP_INIT_ATTR_RESERVED = 1 << 6
};

enum ibv_qp_create_flags {
	IBV_QP_CREATE_BLOCK_SELF_MCAST_LB = 1 << 1,
	IBV_QP_CREATE_SCATTER_FCS = 1 << 8,
	IBV_QP_CREATE_CVLAN_STRIPPING = 1 << 9,
	IBV_QP_CREATE_SOURCE_QPN = 1 << 10,
};

struct ibv_rx_hash_conf {
	uint8_t rx_hash_function;
	uint8_t rx_hash_key_len;
	uint8_t *rx_hash_key;
	uint64_t rx_hash_fields_mask;
};

struct ibv_qp_init_attr_ex {
	void *qp_context;
	struct ibv_cq *send_cq;
	struct ibv_cq *recv_cq;
	struct ibv_srq *srq;
	struct ibv_qp_cap cap;
	enum ibv_qp_type qp_type;
	int sq_sig_all;

	uint32_t comp_mask;
	struct ibv_pd *pd;
	struct ibv_xrcd *xrcd;
	uint32_t create_flags;
	uint16_t max_tso_header;
	struct ibv_rwq_ind_table *rwq_ind_tbl;
	struct ibv_rx_hash_conf rx_hash_conf;
	uint32_t source_qpn;
};

enum ibv_qp_open_attr_mask {
	IBV_QP_OPEN_ATTR_NUM = 1 << 0,
	IBV_QP_OPEN_ATTR_XRCD = 1 << 1,
	IBV_QP_OPEN_ATTR_CONTEXT = 1 << 2,
	IBV_QP_OPEN_ATTR_TYPE = 1 << 3,
	IBV_QP_OPEN_ATTR_RESERVED = 1 << 4
};

struct ibv_qp_open_attr {
	uint32_t comp_mask;
	uint32_t qp_num;
	struct ibv_xrcd *xrcd;
	void *qp_context;
	enum ibv_qp_type qp_type;
};

enum ibv_qp_attr_mask {
	IBV_QP_STATE = 1 << 0,
	IBV_QP_CUR_STATE = 1 << 1,
	IBV_QP_EN_SQD_ASYNC_NOTIFY = 1 << 2,
	IBV_QP_ACCESS_FLAGS = 1 << 3,
	IBV_QP_PKEY_INDEX = 1 << 4,
	IBV_QP_PORT = 1 << 5,
	IBV_QP_QKEY = 1 << 6,
	IBV_QP_AV = 1 << 7,
	IBV_QP_PATH_MTU = 1 << 8,
	IBV_QP_TIMEOUT = 1 << 9,
	IBV_QP_RETRY_CNT = 1 << 10,
	IBV_QP_RNR_RETRY = 1 << 11,
	IBV_QP_RQ_PSN = 1 << 12,
	IBV_QP_MAX_QP_RD_ATOMIC = 1 << 13,
	IBV_QP_ALT_PATH = 1 << 14,
	IBV_QP_MIN_RNR_TIMER = 1 << 15,
	IBV_QP_SQ_PSN = 1 << 16,
	IBV_QP_MAX_DEST_RD_ATOMIC = 1 << 17,
	IBV_QP_PATH_MIG_STATE = 1 << 18,
	IBV_QP_CAP = 1 << 19,
	IBV_QP_DEST_QPN = 1 << 20,
	IBV_QP_RATE_LIMIT = 1 << 25,
};

enum ibv_qp_state {
	IBV_QPS_RESET,
	IBV_QPS_INIT,
	IBV_QPS_RTR,
	IBV_QPS_RTS,
	IBV_QPS_SQD,
	IBV_QPS_SQE,
	IBV_QPS_ERR,
	IBV_QPS_UNKNOWN
};

enum ibv_mig_state {
	IBV_MIG_MIGRATED,
	IBV_MIG_REARM,
	IBV_MIG_ARMED
};

struct ibv_qp_attr {
	enum ibv_qp_state qp_state;
	enum ibv_qp_state cur_qp_state;
	enum ibv_mtu path_mtu;
	enum ibv_mig_state path_mig_state;
	uint32_t qkey;
	uint32_t rq_psn;
	uint32_t sq_psn;
	uint32_t dest_qp_num;
	int qp_access_flags;
	struct ibv_qp_cap cap;
	struct ibv_ah_attr ah_attr;
	struct ibv_ah_attr alt_ah_attr;
	uint16_t pkey_index;
	uint16_t alt_pkey_index;
	uint8_t en_sqd_async_notify;
	uint8_t sq_draining;
	uint8_t max_rd_atomic;
	uint8_t max_dest_rd_atomic;
	uint8_t min_rnr_timer;
	uint8_t port_num;
	uint8_t timeout;
	uint8_t retry_cnt;
	uint8_t rnr_retry;
	uint8_t alt_port_num;
	uint8_t alt_timeout;
	uint32_t rate_limit;
};

enum ibv_wr_opcode {
	IBV_WR_RDMA_WRITE,
	IBV_WR_RDMA_WRITE_WITH_IMM,
	IBV_WR_SEND,
	IBV_WR_SEND_WITH_IMM,
	IBV_WR_RDMA_READ,
	IBV_WR_ATOMIC_CMP_AND_SWP,
	IBV_WR_ATOMIC_FETCH_AND_ADD,
	IBV_WR_LOCAL_INV,
	IBV_WR_BIND_MW,
	IBV_WR_SEND_WITH_INV,
	IBV_WR_TSO,
};

enum ibv_send_flags {
	IBV_SEND_FENCE = 1 << 0,
	IBV_SEND_SIGNALED = 1 << 1,
	IBV_SEND_SOLICITED = 1 << 2,
	IBV_SEND_INLINE = 1 << 3,
	IBV_SEND_IP_CSUM = 1 << 4
};

struct ibv_sge {
	uint64_t addr;
	uint32_t length;
	uint32_t lkey;
};

struct ibv_send_wr {
	uint64_t wr_id;
	struct ibv_send_wr *next;
	struct ibv_sge *sg_list;
	int num_sge;
	enum ibv_wr_opcode opcode;
	int send_flags;
	union {
		__be32 imm_data;
		uint32_t invalidate_rkey;
	};
	union {
		struct {
			uint64_t remote_addr;
			uint32_t rkey;
		} rdma;
		struct {
			uint64_t remote_addr;
			uint64_t compare_add;
			uint64_t swap;
			uint32_t rkey;
		} atomic;
		struct {
			struct ibv_ah *ah;
			uint32_t remote_qpn;
			uint32_t remote_qkey;
		} ud;
	} wr;
	union {
		struct {
			uint32_t remote_srqn;
		} xrc;
	} qp_type;
	union {
		struct {
			struct ibv_mw *mw;
			uint32_t rkey;
			struct ibv_mw_bind_info bind_info;
		} bind_mw;
		struct {
			void *hdr;
			uint16_t hdr_sz;
			uint16_t mss;
		} tso;
	};
};

struct ibv_recv_wr {
	uint64_t wr_id;
	struct ibv_recv_wr *next;
	struct ibv_sge *sg_list;
	int num_sge;
};

struct ibv_mw_bind {
	uint64_t wr_id;
	int send_flags;
	struct ibv_mw_bind_info bind_info;
};

struct ibv_srq {
	struct ibv_context *context;
	void *srq_context;
	struct ibv_pd *pd;
	uint32_t handle;

	pthread_mutex_t mutex;
	pthread_cond_t cond;
	uint32_t events_completed;
};

struct ibv_wq {
	struct ibv_context *context;
	void *wq_context;
	struct ibv_pd *pd;
	struct ibv_cq *cq;
	uint32_t wq_num;
	uint32_t handle;
	enum ibv_wq_state state;
	enum ibv_wq_type wq_type;
	int (*post_recv)(struct ibv_wq *current,
			 struct ibv_recv_wr *recv_wr,
			 struct ibv_recv_wr **bad_recv_wr);
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	uint32_t events_completed;
	uint32_t comp_mask;
};

struct ibv_qp {
	struct ibv_context *context;
	void *qp_context;
	struct ibv_pd *pd;
	struct ibv_cq *send_cq;
	struct ibv_cq *recv_cq;
	struct ibv_srq *srq;
	uint32_t handle;
	uint32_t qp_num;
	enum ibv_qp_state state;
	enum ibv_qp_type qp_type;

	pthread_mutex_t mutex;
	pthread_cond_t cond;
	uint32_t events_completed;
};

struct ibv_comp_channel {
	struct ibv_context *context;
	int fd;
	int refcnt;
};

struct ibv_cq {
	struct ibv_context *context;
	struct ibv_comp_channel *channel;
	void *cq_context;
	uint32_t handle;
	int cqe;

	pthread_mutex_t mutex;
	pthread_cond_t cond;
	uint32_t comp_events_completed;
	uint32_t async_events_completed;
};

struct ibv_poll_cq_attr {
	uint32_t comp_mask;
};

struct ibv_cq_ex {
	struct ibv_context *context;
	struct ibv_comp_channel *channel;
	void *cq_context;
	uint32_t handle;
	int cqe;

	pthread_mutex_t mutex;
	pthread_cond_t cond;
	uint32_t comp_events_completed;
	uint32_t async_events_completed;

	uint32_t comp_mask;
	enum ibv_wc_status status;
	uint64_t wr_id;
	int (*start_poll)(struct ibv_cq_ex *current,
			     struct ibv_poll_cq_attr *attr);
	int (*next_poll)(struct ibv_cq_ex *current);
	void (*end_poll)(struct ibv_cq_ex *current);
	enum ibv_wc_opcode (*read_opcode)(struct ibv_cq_ex *current);
	uint32_t (*read_vendor_err)(struct ibv_cq_ex *current);
	uint32_t (*read_byte_len)(struct ibv_cq_ex *current);
	__be32 (*read_imm_data)(struct ibv_cq_ex *current);
	uint32_t (*read_qp_num)(struct ibv_cq_ex *current);
	uint32_t (*read_src_qp)(struct ibv_cq_ex *current);
	int (*read_wc_flags)(struct ibv_cq_ex *current);
	uint32_t (*read_slid)(struct ibv_cq_ex *current);
	uint8_t (*read_sl)(struct ibv_cq_ex *current);
	uint8_t (*read_dlid_path_bits)(struct ibv_cq_ex *current);
	uint64_t (*read_completion_ts)(struct ibv_cq_ex *current);
	uint16_t (*read_cvlan)(struct ibv_cq_ex *current);
	uint32_t (*read_flow_tag)(struct ibv_cq_ex *current);
};

struct ibv_ah {
	struct ibv_context *context;
	struct ibv_pd *pd;
	uint32_t handle;
};

enum ibv_flow_flags {
	IBV_FLOW_ATTR_FLAGS_ALLOW_LOOP_BACK = 1 << 0,
	IBV_FLOW_ATTR_FLAGS_DONT_TRAP = 1 << 1,
};

enum ibv_flow_attr_type {
	IBV_FLOW_ATTR_NORMAL = 0x0,
	IBV_FLOW_ATTR_ALL_DEFAULT = 0x1,
	IBV_FLOW_ATTR_MC_DEFAULT = 0x2,
	IBV_FLOW_ATTR_SNIFFER = 0x3,
};

enum ibv_flow_spec_type {
	IBV_FLOW_SPEC_ETH = 0x20,
	IBV_FLOW_SPEC_IPV4 = 0x30,
	IBV_FLOW_SPEC_IPV6 = 0x31,
	IBV_FLOW_SPEC_IPV4_EXT = 0x32,
	IBV_FLOW_SPEC_TCP = 0x40,
	IBV_FLOW_SPEC_UDP = 0x41,
	IBV_FLOW_SPEC_VXLAN_TUNNEL = 0x50,
	IBV_FLOW_SPEC_INNER = 0x100,
	IBV_FLOW_SPEC_ACTION_TAG = 0x1000,
	IBV_FLOW_SPEC_ACTION_DROP = 0x1001,
};

struct ibv_flow_eth_filter {
	uint8_t dst_mac[6];
	uint8_t src_mac[6];
	uint16_t ether_type;
	uint16_t vlan_tag;
};

struct ibv_flow_spec_eth {
	enum ibv_flow_spec_type type;
	uint16_t size;
	struct ibv_flow_eth_filter val;
	struct ibv_flow_eth_filter mask;
};

struct ibv_flow_ipv4_filter {
	uint32_t src_ip;
	uint32_t dst_ip;
};

struct ibv_flow_spec_ipv4 {
	enum ibv_flow_spec_type type;
	uint16_t size;
	struct ibv_flow_ipv4_filter val;
	struct ibv_flow_ipv4_filter mask;
};

struct ibv_flow_ipv4_ext_filter {
	uint32_t src_ip;
	uint32_t dst_ip;
	uint8_t proto;
	uint8_t tos;
	uint8_t ttl;
	uint8_t flags;
};

struct ibv_flow_spec_ipv4_ext {
	enum ibv_flow_spec_type type;
	uint16_t size;
	struct ibv_flow_ipv4_ext_filter val;
	struct ibv_flow_ipv4_ext_filter mask;
};

struct ibv_flow_ipv6_filter {
	uint8_t src_ip[16];
	uint8_t dst_ip[16];
	uint32_t flow_label;
	uint8_t next_hdr;
	uint8_t traffic_class;
	uint8_t hop_limit;
};

struct ibv_flow_spec_ipv6 {
	enum ibv_flow_spec_type type;
	uint16_t size;
	struct ibv_flow_ipv6_filter val;
	struct ibv_flow_ipv6_filter mask;
};

struct ibv_flow_tcp_udp_filter {
	uint16_t dst_port;
	uint16_t src_port;
};

struct ibv_flow_spec_tcp_udp {
	enum ibv_flow_spec_type type;
	uint16_t size;
	struct ibv_flow_tcp_udp_filter val;
	struct ibv_flow_tcp_udp_filter mask;
};

struct ibv_flow_tunnel_filter {
	uint32_t tunnel_id;
};

struct ibv_flow_spec_tunnel {
	enum ibv_flow_spec_type type;
	uint16_t size;
	struct ibv_flow_tunnel_filter val;
	struct ibv_flow_tunnel_filter mask;
};

struct ibv_flow_spec_action_tag {
	enum ibv_flow_spec_type type;
	uint16_t size;
	uint32_t tag_id;
};

struct ibv_flow_spec_action_drop {
	enum ibv_flow_spec_type type;
	uint16_t size;
};

struct ibv_flow_spec {
	union {
		struct {
			enum ibv_flow_spec_type type;
			uint16_t size;
		} hdr;
		struct ibv_flow_spec_eth eth;
		struct ibv_flow_spec_ipv4 ipv4;
		struct ibv_flow_spec_tcp_udp tcp_udp;
		struct ibv_flow_spec_ipv4_ext ipv4_ext;
		struct ibv_flow_spec_ipv6 ipv6;
		struct ibv_flow_spec_tunnel tunnel;
		struct ibv_flow_spec_action_tag flow_tag;
		struct ibv_flow_spec_action_drop drop;
	};
};

struct ibv_flow_attr {
	uint32_t comp_mask;
	enum ibv_flow_attr_type type;
	uint16_t size;
	uint16_t priority;
	uint8_t num_of_specs;
	uint8_t port;
	uint32_t flags;
};

struct ibv_flow {
	uint32_t comp_mask;
	struct ibv_context *context;
	uint32_t handle;
};

struct ibv_device;
struct ibv_context;

/* Obsolete, never used, do not touch */
struct _ibv_device_ops {
	struct ibv_context * (*_dummy1)(struct ibv_device *device, int cmd_fd);
	void (*_dummy2)(struct ibv_context *context);
};

enum {
	IBV_SYSFS_NAME_MAX = 64,
	IBV_SYSFS_PATH_MAX = 256
};

struct ibv_device {
	struct _ibv_device_ops _ops;
	enum ibv_node_type node_type;
	enum ibv_transport_type transport_type;
	char name[IBV_SYSFS_NAME_MAX];
	char dev_name[IBV_SYSFS_NAME_MAX];
	char dev_path[IBV_SYSFS_PATH_MAX];
	char ibdev_path[IBV_SYSFS_PATH_MAX];
};

struct ibv_context_ops {
	int (*query_device)(struct ibv_context *context,
					      struct ibv_device_attr *device_attr);
	int (*query_port)(struct ibv_context *context, uint8_t port_num,
					      struct ibv_port_attr *port_attr);
	struct ibv_pd * (*alloc_pd)(struct ibv_context *context);
	int (*dealloc_pd)(struct ibv_pd *pd);
	struct ibv_mr * (*reg_mr)(struct ibv_pd *pd, void *addr, size_t length,
					  int access);
	int (*rereg_mr)(struct ibv_mr *mr,
					    int flags,
					    struct ibv_pd *pd, void *addr,
					    size_t length,
					    int access);
	int (*dereg_mr)(struct ibv_mr *mr);
	struct ibv_mw * (*alloc_mw)(struct ibv_pd *pd, enum ibv_mw_type type);
	int (*bind_mw)(struct ibv_qp *qp, struct ibv_mw *mw,
					   struct ibv_mw_bind *mw_bind);
	int (*dealloc_mw)(struct ibv_mw *mw);
	struct ibv_cq * (*create_cq)(struct ibv_context *context, int cqe,
					     struct ibv_comp_channel *channel,
					     int comp_vector);
	int (*poll_cq)(struct ibv_cq *cq, int num_entries, struct ibv_wc *wc);
	int (*req_notify_cq)(struct ibv_cq *cq, int solicited_only);
	void (*cq_event)(struct ibv_cq *cq);
	int (*resize_cq)(struct ibv_cq *cq, int cqe);
	int (*destroy_cq)(struct ibv_cq *cq);
	struct ibv_srq * (*create_srq)(struct ibv_pd *pd,
					      struct ibv_srq_init_attr *srq_init_attr);
	int (*modify_srq)(struct ibv_srq *srq,
					      struct ibv_srq_attr *srq_attr,
					      int srq_attr_mask);
	int (*query_srq)(struct ibv_srq *srq,
					     struct ibv_srq_attr *srq_attr);
	int (*destroy_srq)(struct ibv_srq *srq);
	int (*post_srq_recv)(struct ibv_srq *srq,
						 struct ibv_recv_wr *recv_wr,
						 struct ibv_recv_wr **bad_recv_wr);
	struct ibv_qp * (*create_qp)(struct ibv_pd *pd, struct ibv_qp_init_attr *attr);
	int (*query_qp)(struct ibv_qp *qp, struct ibv_qp_attr *attr,
					    int attr_mask,
					    struct ibv_qp_init_attr *init_attr);
	int (*modify_qp)(struct ibv_qp *qp, struct ibv_qp_attr *attr,
					     int attr_mask);
	int (*destroy_qp)(struct ibv_qp *qp);
	int (*post_send)(struct ibv_qp *qp, struct ibv_send_wr *wr,
					     struct ibv_send_wr **bad_wr);
	int (*post_recv)(struct ibv_qp *qp, struct ibv_recv_wr *wr,
					     struct ibv_recv_wr **bad_wr);
	struct ibv_ah * (*create_ah)(struct ibv_pd *pd, struct ibv_ah_attr *attr);
	int (*destroy_ah)(struct ibv_ah *ah);
	int (*attach_mcast)(struct ibv_qp *qp, const union ibv_gid *gid,
						uint16_t lid);
	int (*detach_mcast)(struct ibv_qp *qp, const union ibv_gid *gid,
						uint16_t lid);
	void (*async_event)(struct ibv_async_event *event);
};

struct ibv_context {
	struct ibv_device *device;
	struct ibv_context_ops ops;
	int cmd_fd;
	int async_fd;
	int num_comp_vectors;
	pthread_mutex_t mutex;
	void *abi_compat;
};

enum ibv_cq_init_attr_mask {
	IBV_CQ_INIT_ATTR_MASK_FLAGS = 1 << 0,
	IBV_CQ_INIT_ATTR_MASK_RESERVED = 1 << 1
};

enum ibv_create_cq_attr_flags {
	IBV_CREATE_CQ_ATTR_SINGLE_THREADED = 1 << 0,
	IBV_CREATE_CQ_ATTR_RESERVED = 1 << 1,
};

struct ibv_cq_init_attr_ex {
	uint32_t cqe;
	void *cq_context;
	struct ibv_comp_channel *channel;
	uint32_t comp_vector;
	uint64_t wc_flags;
	uint32_t comp_mask;
	uint32_t flags;
};

enum ibv_values_mask {
	IBV_VALUES_MASK_RAW_CLOCK = 1 << 0,
	IBV_VALUES_MASK_RESERVED = 1 << 1
};

struct ibv_values_ex {
	uint32_t comp_mask;
	struct timespec raw_clock;
};

enum verbs_context_mask {
	VERBS_CONTEXT_XRCD = 1 << 0,
	VERBS_CONTEXT_SRQ = 1 << 1,
	VERBS_CONTEXT_QP = 1 << 2,
	VERBS_CONTEXT_CREATE_FLOW = 1 << 3,
	VERBS_CONTEXT_DESTROY_FLOW = 1 << 4,
	VERBS_CONTEXT_RESERVED = 1 << 5
};

struct verbs_context {
	/* "grows up" - new fields go here */
	int (*destroy_rwq_ind_table)(struct ibv_rwq_ind_table *rwq_ind_table);
	struct ibv_rwq_ind_table *(*create_rwq_ind_table)(struct ibv_context *context,
							  struct ibv_rwq_ind_table_init_attr *init_attr);
	int (*destroy_wq)(struct ibv_wq *wq);
	int (*modify_wq)(struct ibv_wq *wq, struct ibv_wq_attr *wq_attr);
	struct ibv_wq * (*create_wq)(struct ibv_context *context,
				     struct ibv_wq_init_attr *wq_init_attr);
	int (*query_rt_values)(struct ibv_context *context,
			       struct ibv_values_ex *values);
	struct ibv_cq_ex *(*create_cq_ex)(struct ibv_context *context,
					  struct ibv_cq_init_attr_ex *init_attr);
	struct verbs_ex_private *priv;
	int (*query_device_ex)(struct ibv_context *context,
			       const struct ibv_query_device_ex_input *input,
			       struct ibv_device_attr_ex *attr,
			       size_t attr_size);
	int (*ibv_destroy_flow) (struct ibv_flow *flow);
	void (*ABI_placeholder2) (void); /* DO NOT COPY THIS GARBAGE */
	struct ibv_flow * (*ibv_create_flow) (struct ibv_qp *qp,
					      struct ibv_flow_attr *flow_attr);
	void (*ABI_placeholder1) (void); /* DO NOT COPY THIS GARBAGE */
	struct ibv_qp *(*open_qp)(struct ibv_context *context,
			struct ibv_qp_open_attr *attr);
	struct ibv_qp *(*create_qp_ex)(struct ibv_context *context,
			struct ibv_qp_init_attr_ex *qp_init_attr_ex);
	int (*get_srq_num)(struct ibv_srq *srq, uint32_t *srq_num);
	struct ibv_srq * (*create_srq_ex)(struct ibv_context *context,
						 struct ibv_srq_init_attr_ex *srq_init_attr_ex);
	struct ibv_xrcd * (*open_xrcd)(struct ibv_context *context,
					     struct ibv_xrcd_init_attr *xrcd_init_attr);
	int (*close_xrcd)(struct ibv_xrcd *xrcd);
	uint64_t has_comp_mask;
	size_t sz; /* Must be immediately before struct ibv_context */
	struct ibv_context context; /* Must be last field in the struct */
};

#define ETHERNET_LL_SIZE 6


/*
 * Verbs function declarations
 */

// TODO: This is a small workaround while the verbs API is not entirely transparent, i.e. while we
// have to use the guest_to_host conversion function in our user space application. This way we
// don't need to include page.h in the application.
size_t guest_to_host(size_t address);

const char * ibv_wc_status_str(enum ibv_wc_status status);
int ibv_rate_to_mult(enum ibv_rate rate);
enum ibv_rate mult_to_ibv_rate(int mult);
int ibv_rate_to_mbps(enum ibv_rate rate);
enum ibv_rate mbps_to_ibv_rate(int mbps);
struct ibv_cq * ibv_cq_ex_to_cq(struct ibv_cq_ex * cq);
int ibv_start_poll(struct ibv_cq_ex * cq, struct ibv_poll_cq_attr * attr);
int ibv_next_poll(struct ibv_cq_ex * cq);
void ibv_end_poll(struct ibv_cq_ex * cq);
enum ibv_wc_opcode ibv_wc_read_opcode(struct ibv_cq_ex * cq);
uint32_t ibv_wc_read_vendor_err(struct ibv_cq_ex * cq);
uint32_t ibv_wc_read_byte_len(struct ibv_cq_ex * cq);
__be32 ibv_wc_read_imm_data(struct ibv_cq_ex * cq);
uint32_t ibv_wc_read_invalidated_rkey(struct ibv_cq_ex * cq);
uint32_t ibv_wc_read_qp_num(struct ibv_cq_ex * cq);
uint32_t ibv_wc_read_src_qp(struct ibv_cq_ex * cq);
int ibv_wc_read_wc_flags(struct ibv_cq_ex * cq);
uint32_t ibv_wc_read_slid(struct ibv_cq_ex * cq);
uint8_t ibv_wc_read_sl(struct ibv_cq_ex * cq);
uint8_t ibv_wc_read_dlid_path_bits(struct ibv_cq_ex * cq);
uint64_t ibv_wc_read_completion_ts(struct ibv_cq_ex * cq);
uint16_t ibv_wc_read_cvlan(struct ibv_cq_ex * cq);
uint32_t ibv_wc_read_flow_tag(struct ibv_cq_ex * cq);
struct ibv_device ** ibv_get_device_list(int * num_devices);
int ibv_post_wq_recv(struct ibv_wq * wq, struct ibv_recv_wr * recv_wr, struct ibv_recv_wr ** bad_recv_wr);
void ibv_free_device_list(struct ibv_device ** list);
const char * ibv_get_device_name(struct ibv_device * device);
__be64 ibv_get_device_guid(struct ibv_device * device);
struct ibv_context * ibv_open_device(struct ibv_device * device);
int ibv_close_device(struct ibv_context * context);
int ibv_get_async_event(struct ibv_context * context, struct ibv_async_event * event);
void ibv_ack_async_event(struct ibv_async_event * event);
int ibv_query_device(struct ibv_context * context, struct ibv_device_attr * device_attr);
int ibv_query_gid(struct ibv_context * context, uint8_t port_num, int index, union ibv_gid * gid);
int ibv_query_port(struct ibv_context * context, uint8_t port_num, struct ibv_port_attr * port_attr);
int ibv_query_pkey(struct ibv_context * context, uint8_t port_num, int index, __be16 * pkey);
struct ibv_pd * ibv_alloc_pd(struct ibv_context * context);
int ibv_dealloc_pd(struct ibv_pd * pd);
struct ibv_flow * ibv_create_flow(struct ibv_qp * qp, struct ibv_flow_attr * flow);
int ibv_destroy_flow(struct ibv_flow * flow_id);
struct ibv_xrcd * ibv_open_xrcd(struct ibv_context * context, struct ibv_xrcd_init_attr * xrcd_init_attr);
int ibv_close_xrcd(struct ibv_xrcd * xrcd);
struct ibv_mr * ibv_reg_mr(struct ibv_pd * pd, void * addr, int length, int access);
int ibv_rereg_mr(struct ibv_mr * mr, int flags, struct ibv_pd * pd, void * addr, int length, int access);
int ibv_dereg_mr(struct ibv_mr * mr);
struct ibv_mw * ibv_alloc_mw(struct ibv_pd * pd, enum ibv_mw_type type);
int ibv_dealloc_mw(struct ibv_mw * mw);
uint32_t ibv_inc_rkey(uint32_t rkey);
int ibv_bind_mw(struct ibv_qp * qp, struct ibv_mw * mw, struct ibv_mw_bind * mw_bind);
struct ibv_comp_channel * ibv_create_comp_channel(struct ibv_context * context);
int ibv_destroy_comp_channel(struct ibv_comp_channel * channel);
struct ibv_cq * ibv_create_cq(struct ibv_context * context, int cqe, void * cq_context, struct ibv_comp_channel * channel, int comp_vector);
struct ibv_cq_ex * ibv_create_cq_ex(struct ibv_context * context, struct ibv_cq_init_attr_ex * cq_attr);
int ibv_resize_cq(struct ibv_cq * cq, int cqe);
int ibv_destroy_cq(struct ibv_cq * cq);
int ibv_get_cq_event(struct ibv_comp_channel * channel, struct ibv_cq ** cq, void ** cq_context);
void ibv_ack_cq_events(struct ibv_cq * cq, unsigned int nevents);
int ibv_poll_cq(struct ibv_cq * cq, int num_entries, struct ibv_wc * wc);
int ibv_req_notify_cq(struct ibv_cq * cq, int solicited_only);
struct ibv_srq * ibv_create_srq(struct ibv_pd * pd, struct ibv_srq_init_attr * srq_init_attr);
struct ibv_srq * ibv_create_srq_ex(struct ibv_context * context, struct ibv_srq_init_attr_ex * srq_init_attr_ex);
int ibv_modify_srq(struct ibv_srq * srq, struct ibv_srq_attr * srq_attr, int srq_attr_mask);
int ibv_query_srq(struct ibv_srq * srq, struct ibv_srq_attr * srq_attr);
int ibv_get_srq_num(struct ibv_srq * srq, uint32_t * srq_num);
int ibv_destroy_srq(struct ibv_srq * srq);
int ibv_post_srq_recv(struct ibv_srq * srq, struct ibv_recv_wr * recv_wr, struct ibv_recv_wr ** bad_recv_wr);
struct ibv_qp * ibv_create_qp(struct ibv_pd * pd, struct ibv_qp_init_attr * qp_init_attr);
struct ibv_qp * ibv_create_qp_ex(struct ibv_context * context, struct ibv_qp_init_attr_ex * qp_init_attr_ex);
int ibv_query_rt_values_ex(struct ibv_context * context, struct ibv_values_ex * values);
int ibv_query_device_ex(struct ibv_context * context, const struct ibv_query_device_ex_input * input, struct ibv_device_attr_ex * attr);
struct ibv_qp * ibv_open_qp(struct ibv_context * context, struct ibv_qp_open_attr * qp_open_attr);
int ibv_modify_qp(struct ibv_qp * qp, struct ibv_qp_attr * attr, int attr_mask);
int ibv_query_qp(struct ibv_qp * qp, struct ibv_qp_attr * attr, int attr_mask, struct ibv_qp_init_attr * init_attr);
int ibv_destroy_qp(struct ibv_qp * qp);
struct ibv_wq * ibv_create_wq(struct ibv_context * context, struct ibv_wq_init_attr * wq_init_attr);
int ibv_modify_wq(struct ibv_wq * wq, struct ibv_wq_attr * wq_attr);
int ibv_destroy_wq(struct ibv_wq * wq);
struct ibv_rwq_ind_table * ibv_create_rwq_ind_table(struct ibv_context * context, struct ibv_rwq_ind_table_init_attr * init_attr);
int ibv_destroy_rwq_ind_table(struct ibv_rwq_ind_table * rwq_ind_table);
int ibv_post_send(struct ibv_qp * qp, struct ibv_send_wr * wr, struct ibv_send_wr ** bad_wr);
int ibv_post_recv(struct ibv_qp * qp, struct ibv_recv_wr * recv_wr, struct ibv_recv_wr ** bad_recv_wr);
struct ibv_ah * ibv_create_ah(struct ibv_pd * pd, struct ibv_ah_attr * attr);
int ibv_init_ah_from_wc(struct ibv_context * context, uint8_t port_num, struct ibv_wc * wc, struct ibv_grh * grh, struct ibv_ah_attr * ah_attr);
struct ibv_ah * ibv_create_ah_from_wc(struct ibv_pd * pd, struct ibv_wc * wc, struct ibv_grh * grh, uint8_t port_num);
int ibv_destroy_ah(struct ibv_ah * ah);
int ibv_attach_mcast(struct ibv_qp * qp, const union ibv_gid * gid, uint16_t lid);
int ibv_detach_mcast(struct ibv_qp * qp, const union ibv_gid * gid, uint16_t lid);
int ibv_fork_init( );
const char * ibv_node_type_str(enum ibv_node_type node_type);
const char * ibv_port_state_str(enum ibv_port_state port_state);
const char * ibv_event_type_str(enum ibv_event_type event);
// TODO: Fix problem with this function.
// int ibv_resolve_eth_l2_from_gid(struct ibv_context * context, struct ibv_ah_attr * attr,
                                // uint8_t eth_mac[ETHERNET_LL_SIZE], uint16_t * vid);
int ibv_is_qpt_supported(uint32_t caps, enum ibv_qp_type qpt);

#ifdef __cplusplus
}
#endif

#endif /* __HERMIT_VERBS_H__ */
