/* SPDX-License-Identifier: BSD-3-Clause-Clear */
/*
 * Copyright (c) 2018-2021 The Linux Foundation. All rights reserved.
 * Copyright (c) 2021-2025 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#ifndef DEBUG_HTT_STATS_H
#define DEBUG_HTT_STATS_H

#include <stdint.h>
#include <stdbool.h>

#define __packed

#define DECLARE_FLEX_ARRAY(type, name) int ____dummy; type name[]

#define ATH12K_HTT_STATS_BUF_SIZE		(1024 * 512)
#define ATH12K_HTT_STATS_COOKIE_LSB		GENMASK_ULL(31, 0)
#define ATH12K_HTT_STATS_COOKIE_MSB		GENMASK_ULL(63, 32)
#define ATH12K_HTT_STATS_MAGIC_VALUE		0xF0F0F0F0
#define ATH12K_HTT_STATS_SUBTYPE_MAX		16
#define ATH12K_HTT_MAX_STRING_LEN		256

#define ATH12K_HTT_STATS_RESET_BITMAP32_OFFSET(_idx)	((_idx) & 0x1f)
#define ATH12K_HTT_STATS_RESET_BITMAP64_OFFSET(_idx)	((_idx) & 0x3f)
#define ATH12K_HTT_STATS_RESET_BITMAP32_BIT(_idx)	(1 << \
		ATH12K_HTT_STATS_RESET_BITMAP32_OFFSET(_idx))
#define ATH12K_HTT_STATS_RESET_BITMAP64_BIT(_idx)	(1 << \
		ATH12K_HTT_STATS_RESET_BITMAP64_OFFSET(_idx))

struct ath12k {
    int dummy;
};

struct ath12k_base {
    int dummy;
};

/**
 * DOC: target -> host extended statistics upload
 *
 * The following field definitions describe the format of the HTT
 * target to host stats upload confirmation message.
 * The message contains a cookie echoed from the HTT host->target stats
 * upload request, which identifies which request the confirmation is
 * for, and a single stats can span over multiple HTT stats indication
 * due to the HTT message size limitation so every HTT ext stats
 * indication will have tag-length-value stats information elements.
 * The tag-length header for each HTT stats IND message also includes a
 * status field, to indicate whether the request for the stat type in
 * question was fully met, partially met, unable to be met, or invalid
 * (if the stat type in question is disabled in the target).
 * A Done bit 1's indicate the end of the of stats info elements.
 *
 *
 * |31                         16|15    12|11|10 8|7   5|4       0|
 * |--------------------------------------------------------------|
 * |                   reserved                   |    msg type   |
 * |--------------------------------------------------------------|
 * |                         cookie LSBs                          |
 * |--------------------------------------------------------------|
 * |                         cookie MSBs                          |
 * |--------------------------------------------------------------|
 * |      stats entry length     | rsvd   | D|  S |   stat type   |
 * |--------------------------------------------------------------|
 * |                   type-specific stats info                   |
 * |                      (see debugfs_htt_stats.h)               |
 * |--------------------------------------------------------------|
 * Header fields:
 *  - MSG_TYPE
 *    Bits 7:0
 *    Purpose: Identifies this is a extended statistics upload confirmation
 *             message.
 *    Value: 0x1c
 *  - COOKIE_LSBS
 *    Bits 31:0
 *    Purpose: Provide a mechanism to match a target->host stats confirmation
 *        message with its preceding host->target stats request message.
 *    Value: MSBs of the opaque cookie specified by the host-side requestor
 *  - COOKIE_MSBS
 *    Bits 31:0
 *    Purpose: Provide a mechanism to match a target->host stats confirmation
 *        message with its preceding host->target stats request message.
 *    Value: MSBs of the opaque cookie specified by the host-side requestor
 *
 * Stats Information Element tag-length header fields:
 *  - STAT_TYPE
 *    Bits 7:0
 *    Purpose: identifies the type of statistics info held in the
 *        following information element
 *    Value: ath12k_dbg_htt_ext_stats_type
 *  - STATUS
 *    Bits 10:8
 *    Purpose: indicate whether the requested stats are present
 *    Value:
 *       0 -> The requested stats have been delivered in full
 *       1 -> The requested stats have been delivered in part
 *       2 -> The requested stats could not be delivered (error case)
 *       3 -> The requested stat type is either not recognized (invalid)
 *  - DONE
 *    Bits 11
 *    Purpose:
 *        Indicates the completion of the stats entry, this will be the last
 *        stats conf HTT segment for the requested stats type.
 *    Value:
 *        0 -> the stats retrieval is ongoing
 *        1 -> the stats retrieval is complete
 *  - LENGTH
 *    Bits 31:16
 *    Purpose: indicate the stats information size
 *    Value: This field specifies the number of bytes of stats information
 *       that follows the element tag-length header.
 *       It is expected but not required that this length is a multiple of
 *       4 bytes.
 */

#define ATH12K_HTT_T2H_EXT_STATS_INFO1_DONE		BIT(11)
#define ATH12K_HTT_T2H_EXT_STATS_INFO1_LENGTH		GENMASK(31, 16)

struct ath12k_htt_extd_stats_msg {
	uint32_t info0;
	uint64_t cookie;
	uint32_t info1;
	uint8_t data[];
} __packed;

/* htt_dbg_ext_stats_type */
enum ath12k_dbg_htt_ext_stats_type {
	ATH12K_DBG_HTT_EXT_STATS_RESET				= 0,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_TX			= 1,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_TX_SCHED			= 4,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_ERROR			= 5,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_TQM			= 6,
	ATH12K_DBG_HTT_EXT_STATS_TX_DE_INFO			= 8,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_TX_RATE			= 9,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_RX_RATE			= 10,
	ATH12K_DBG_HTT_EXT_STATS_TX_SELFGEN_INFO		= 12,
	ATH12K_DBG_HTT_EXT_STATS_SRNG_INFO			= 15,
	ATH12K_DBG_HTT_EXT_STATS_SFM_INFO			= 16,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_TX_MU			= 17,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_CCA_STATS			= 19,
	ATH12K_DBG_HTT_EXT_STATS_TX_SOUNDING_INFO		= 22,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_OBSS_PD_STATS		= 23,
	ATH12K_DBG_HTT_EXT_STATS_LATENCY_PROF_STATS		= 25,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_UL_TRIG_STATS		= 26,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_UL_MUMIMO_TRIG_STATS	= 27,
	ATH12K_DBG_HTT_EXT_STATS_FSE_RX				= 28,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_RX_RATE_EXT		= 30,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_TX_RATE_TXBF		= 31,
	ATH12K_DBG_HTT_EXT_STATS_TXBF_OFDMA			= 32,
	ATH12K_DBG_HTT_EXT_STATS_DLPAGER_STATS			= 36,
	ATH12K_DBG_HTT_EXT_PHY_COUNTERS_AND_PHY_STATS		= 37,
	ATH12K_DBG_HTT_EXT_VDEVS_TXRX_STATS			= 38,
	ATH12K_DBG_HTT_EXT_PDEV_PER_STATS			= 40,
	ATH12K_DBG_HTT_EXT_AST_ENTRIES				= 41,
	ATH12K_DBG_HTT_EXT_STATS_SOC_ERROR			= 45,
	ATH12K_DBG_HTT_DBG_PDEV_PUNCTURE_STATS			= 46,
	ATH12K_DBG_HTT_EXT_STATS_PDEV_SCHED_ALGO		= 49,
	ATH12K_DBG_HTT_EXT_STATS_MANDATORY_MUOFDMA		= 51,
	ATH12K_DGB_HTT_EXT_STATS_PDEV_MBSSID_CTRL_FRAME		= 54,

	/* keep this last */
	ATH12K_DBG_HTT_NUM_EXT_STATS,
};

enum ath12k_dbg_htt_tlv_tag {
	HTT_STATS_TX_PDEV_CMN_TAG			= 0,
	HTT_STATS_TX_PDEV_UNDERRUN_TAG			= 1,
	HTT_STATS_TX_PDEV_SIFS_TAG			= 2,
	HTT_STATS_TX_PDEV_FLUSH_TAG			= 3,
	HTT_STATS_STRING_TAG				= 5,
	HTT_STATS_TX_TQM_GEN_MPDU_TAG			= 11,
	HTT_STATS_TX_TQM_LIST_MPDU_TAG			= 12,
	HTT_STATS_TX_TQM_LIST_MPDU_CNT_TAG		= 13,
	HTT_STATS_TX_TQM_CMN_TAG			= 14,
	HTT_STATS_TX_TQM_PDEV_TAG			= 15,
	HTT_STATS_TX_DE_EAPOL_PACKETS_TAG		= 17,
	HTT_STATS_TX_DE_CLASSIFY_FAILED_TAG		= 18,
	HTT_STATS_TX_DE_CLASSIFY_STATS_TAG		= 19,
	HTT_STATS_TX_DE_CLASSIFY_STATUS_TAG		= 20,
	HTT_STATS_TX_DE_ENQUEUE_PACKETS_TAG		= 21,
	HTT_STATS_TX_DE_ENQUEUE_DISCARD_TAG		= 22,
	HTT_STATS_TX_DE_CMN_TAG				= 23,
	HTT_STATS_TX_PDEV_MU_MIMO_STATS_TAG		= 25,
	HTT_STATS_SFM_CMN_TAG				= 26,
	HTT_STATS_SRING_STATS_TAG			= 27,
	HTT_STATS_TX_PDEV_RATE_STATS_TAG		= 34,
	HTT_STATS_RX_PDEV_RATE_STATS_TAG		= 35,
	HTT_STATS_TX_PDEV_SCHEDULER_TXQ_STATS_TAG	= 36,
	HTT_STATS_TX_SCHED_CMN_TAG			= 37,
	HTT_STATS_SCHED_TXQ_CMD_POSTED_TAG		= 39,
	HTT_STATS_SFM_CLIENT_USER_TAG			= 41,
	HTT_STATS_SFM_CLIENT_TAG			= 42,
	HTT_STATS_TX_TQM_ERROR_STATS_TAG                = 43,
	HTT_STATS_SCHED_TXQ_CMD_REAPED_TAG		= 44,
	HTT_STATS_TX_SELFGEN_AC_ERR_STATS_TAG		= 46,
	HTT_STATS_TX_SELFGEN_CMN_STATS_TAG		= 47,
	HTT_STATS_TX_SELFGEN_AC_STATS_TAG		= 48,
	HTT_STATS_TX_SELFGEN_AX_STATS_TAG		= 49,
	HTT_STATS_TX_SELFGEN_AX_ERR_STATS_TAG		= 50,
	HTT_STATS_HW_INTR_MISC_TAG			= 54,
	HTT_STATS_HW_PDEV_ERRS_TAG			= 56,
	HTT_STATS_TX_DE_COMPL_STATS_TAG			= 65,
	HTT_STATS_WHAL_TX_TAG				= 66,
	HTT_STATS_TX_PDEV_SIFS_HIST_TAG			= 67,
	HTT_STATS_PDEV_CCA_1SEC_HIST_TAG		= 70,
	HTT_STATS_PDEV_CCA_100MSEC_HIST_TAG		= 71,
	HTT_STATS_PDEV_CCA_STAT_CUMULATIVE_TAG		= 72,
	HTT_STATS_PDEV_CCA_COUNTERS_TAG			= 73,
	HTT_STATS_TX_PDEV_MPDU_STATS_TAG		= 74,
	HTT_STATS_TX_SOUNDING_STATS_TAG			= 80,
	HTT_STATS_SCHED_TXQ_SCHED_ORDER_SU_TAG		= 86,
	HTT_STATS_SCHED_TXQ_SCHED_INELIGIBILITY_TAG	= 87,
	HTT_STATS_PDEV_OBSS_PD_TAG			= 88,
	HTT_STATS_HW_WAR_TAG				= 89,
	HTT_STATS_LATENCY_PROF_STATS_TAG		= 91,
	HTT_STATS_LATENCY_CTX_TAG			= 92,
	HTT_STATS_LATENCY_CNT_TAG			= 93,
	HTT_STATS_RX_PDEV_UL_TRIG_STATS_TAG		= 94,
	HTT_STATS_RX_PDEV_UL_OFDMA_USER_STATS_TAG	= 95,
	HTT_STATS_RX_PDEV_UL_MUMIMO_TRIG_STATS_TAG	= 97,
	HTT_STATS_RX_FSE_STATS_TAG			= 98,
	HTT_STATS_SCHED_TXQ_SUPERCYCLE_TRIGGER_TAG	= 100,
	HTT_STATS_PDEV_CTRL_PATH_TX_STATS_TAG		= 102,
	HTT_STATS_RX_PDEV_RATE_EXT_STATS_TAG		= 103,
	HTT_STATS_PDEV_TX_RATE_TXBF_STATS_TAG		= 108,
	HTT_STATS_TX_SELFGEN_AC_SCHED_STATUS_STATS_TAG	= 111,
	HTT_STATS_TX_SELFGEN_AX_SCHED_STATUS_STATS_TAG	= 112,
	HTT_STATS_DLPAGER_STATS_TAG			= 120,
	HTT_STATS_PHY_COUNTERS_TAG			= 121,
	HTT_STATS_PHY_STATS_TAG				= 122,
	HTT_STATS_PHY_RESET_COUNTERS_TAG		= 123,
	HTT_STATS_PHY_RESET_STATS_TAG			= 124,
	HTT_STATS_SOC_TXRX_STATS_COMMON_TAG		= 125,
	HTT_STATS_PER_RATE_STATS_TAG			= 128,
	HTT_STATS_MU_PPDU_DIST_TAG			= 129,
	HTT_STATS_TX_PDEV_MUMIMO_GRP_STATS_TAG		= 130,
	HTT_STATS_AST_ENTRY_TAG				= 132,
	HTT_STATS_TX_PDEV_RATE_STATS_BE_OFDMA_TAG	= 135,
	HTT_STATS_TX_SELFGEN_BE_ERR_STATS_TAG		= 137,
	HTT_STATS_TX_SELFGEN_BE_STATS_TAG		= 138,
	HTT_STATS_TX_SELFGEN_BE_SCHED_STATUS_STATS_TAG	= 139,
	HTT_STATS_TXBF_OFDMA_AX_NDPA_STATS_TAG		= 147,
	HTT_STATS_TXBF_OFDMA_AX_NDP_STATS_TAG		= 148,
	HTT_STATS_TXBF_OFDMA_AX_BRP_STATS_TAG		= 149,
	HTT_STATS_TXBF_OFDMA_AX_STEER_STATS_TAG		= 150,
	HTT_STATS_DMAC_RESET_STATS_TAG			= 155,
	HTT_STATS_PHY_TPC_STATS_TAG			= 157,
	HTT_STATS_PDEV_PUNCTURE_STATS_TAG		= 158,
	HTT_STATS_PDEV_SCHED_ALGO_OFDMA_STATS_TAG	= 165,
	HTT_STATS_TXBF_OFDMA_AX_STEER_MPDU_STATS_TAG	= 172,
	HTT_STATS_PDEV_MBSSID_CTRL_FRAME_STATS_TAG	= 176,

	HTT_STATS_MAX_TAG,
};

#define ATH12K_HTT_STATS_MAC_ID				GENMASK(7, 0)

#define ATH12K_HTT_TX_PDEV_MAX_SIFS_BURST_STATS		9
#define ATH12K_HTT_TX_PDEV_MAX_FLUSH_REASON_STATS	150

/* MU MIMO distribution stats is a 2-dimensional array
 * with dimension one denoting stats for nr4[0] or nr8[1]
 */
#define ATH12K_HTT_STATS_NUM_NR_BINS			2
#define ATH12K_HTT_STATS_MAX_NUM_MU_PPDU_PER_BURST	10
#define ATH12K_HTT_TX_PDEV_MAX_SIFS_BURST_HIST_STATS	10
#define ATH12K_HTT_STATS_MAX_NUM_SCHED_STATUS		9
#define ATH12K_HTT_STATS_NUM_SCHED_STATUS_WORDS		\
	(ATH12K_HTT_STATS_NUM_NR_BINS * ATH12K_HTT_STATS_MAX_NUM_SCHED_STATUS)
#define ATH12K_HTT_STATS_MU_PPDU_PER_BURST_WORDS	\
	(ATH12K_HTT_STATS_NUM_NR_BINS * ATH12K_HTT_STATS_MAX_NUM_MU_PPDU_PER_BURST)

enum ath12k_htt_tx_pdev_underrun_enum {
	HTT_STATS_TX_PDEV_NO_DATA_UNDERRUN		= 0,
	HTT_STATS_TX_PDEV_DATA_UNDERRUN_BETWEEN_MPDU	= 1,
	HTT_STATS_TX_PDEV_DATA_UNDERRUN_WITHIN_MPDU	= 2,
	HTT_TX_PDEV_MAX_URRN_STATS			= 3,
};

enum ath12k_htt_stats_reset_cfg_param_alloc_pos {
	ATH12K_HTT_STATS_RESET_PARAM_CFG_32_BYTES = 1,
	ATH12K_HTT_STATS_RESET_PARAM_CFG_64_BYTES,
	ATH12K_HTT_STATS_RESET_PARAM_CFG_128_BYTES,
};

#define ETH_ALEN 12

struct debug_htt_stats_req {
	bool done;
	bool override_cfg_param;
	uint8_t pdev_id;
	enum ath12k_dbg_htt_ext_stats_type type;
	uint32_t cfg_param[4];
	uint8_t peer_addr[ETH_ALEN];
	//struct completion htt_stats_rcvd;
	uint32_t buf_len;
	uint8_t buf[];
};

struct ath12k_htt_tx_pdev_stats_cmn_tlv {
	uint32_t mac_id__word;
	uint32_t hw_queued;
	uint32_t hw_reaped;
	uint32_t underrun;
	uint32_t hw_paused;
	uint32_t hw_flush;
	uint32_t hw_filt;
	uint32_t tx_abort;
	uint32_t mpdu_requed;
	uint32_t tx_xretry;
	uint32_t data_rc;
	uint32_t mpdu_dropped_xretry;
	uint32_t illgl_rate_phy_err;
	uint32_t cont_xretry;
	uint32_t tx_timeout;
	uint32_t pdev_resets;
	uint32_t phy_underrun;
	uint32_t txop_ovf;
	uint32_t seq_posted;
	uint32_t seq_failed_queueing;
	uint32_t seq_completed;
	uint32_t seq_restarted;
	uint32_t mu_seq_posted;
	uint32_t seq_switch_hw_paused;
	uint32_t next_seq_posted_dsr;
	uint32_t seq_posted_isr;
	uint32_t seq_ctrl_cached;
	uint32_t mpdu_count_tqm;
	uint32_t msdu_count_tqm;
	uint32_t mpdu_removed_tqm;
	uint32_t msdu_removed_tqm;
	uint32_t mpdus_sw_flush;
	uint32_t mpdus_hw_filter;
	uint32_t mpdus_truncated;
	uint32_t mpdus_ack_failed;
	uint32_t mpdus_expired;
	uint32_t mpdus_seq_hw_retry;
	uint32_t ack_tlv_proc;
	uint32_t coex_abort_mpdu_cnt_valid;
	uint32_t coex_abort_mpdu_cnt;
	uint32_t num_total_ppdus_tried_ota;
	uint32_t num_data_ppdus_tried_ota;
	uint32_t local_ctrl_mgmt_enqued;
	uint32_t local_ctrl_mgmt_freed;
	uint32_t local_data_enqued;
	uint32_t local_data_freed;
	uint32_t mpdu_tried;
	uint32_t isr_wait_seq_posted;

	uint32_t tx_active_dur_us_low;
	uint32_t tx_active_dur_us_high;
	uint32_t remove_mpdus_max_retries;
	uint32_t comp_delivered;
	uint32_t ppdu_ok;
	uint32_t self_triggers;
	uint32_t tx_time_dur_data;
	uint32_t seq_qdepth_repost_stop;
	uint32_t mu_seq_min_msdu_repost_stop;
	uint32_t seq_min_msdu_repost_stop;
	uint32_t seq_txop_repost_stop;
	uint32_t next_seq_cancel;
	uint32_t fes_offsets_err_cnt;
	uint32_t num_mu_peer_blacklisted;
	uint32_t mu_ofdma_seq_posted;
	uint32_t ul_mumimo_seq_posted;
	uint32_t ul_ofdma_seq_posted;

	uint32_t thermal_suspend_cnt;
	uint32_t dfs_suspend_cnt;
	uint32_t tx_abort_suspend_cnt;
	uint32_t tgt_specific_opaque_txq_suspend_info;
	uint32_t last_suspend_reason;
} __packed;

struct ath12k_htt_tx_pdev_stats_urrn_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, urrn_stats);
} __packed;

struct ath12k_htt_tx_pdev_stats_flush_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, flush_errs);
} __packed;

struct ath12k_htt_tx_pdev_stats_phy_err_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, phy_errs);
} __packed;

struct ath12k_htt_tx_pdev_stats_sifs_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, sifs_status);
} __packed;

struct ath12k_htt_pdev_ctrl_path_tx_stats_tlv {
	uint32_t fw_tx_mgmt_subtype[ATH12K_HTT_STATS_SUBTYPE_MAX];
} __packed;

struct ath12k_htt_tx_pdev_stats_sifs_hist_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, sifs_hist_status);
} __packed;

enum ath12k_htt_stats_hw_mode {
	ATH12K_HTT_STATS_HWMODE_AC = 0,
	ATH12K_HTT_STATS_HWMODE_AX = 1,
	ATH12K_HTT_STATS_HWMODE_BE = 2,
};

struct ath12k_htt_tx_pdev_mu_ppdu_dist_stats_tlv {
	uint32_t hw_mode;
	uint32_t num_seq_term_status[ATH12K_HTT_STATS_NUM_SCHED_STATUS_WORDS];
	uint32_t num_ppdu_cmpl_per_burst[ATH12K_HTT_STATS_MU_PPDU_PER_BURST_WORDS];
	uint32_t num_seq_posted[ATH12K_HTT_STATS_NUM_NR_BINS];
	uint32_t num_ppdu_posted_per_burst[ATH12K_HTT_STATS_MU_PPDU_PER_BURST_WORDS];
} __packed;

#define ATH12K_HTT_TX_PDEV_STATS_NUM_MCS_COUNTERS        12
#define ATH12K_HTT_TX_PDEV_STATS_NUM_GI_COUNTERS          4
#define ATH12K_HTT_TX_PDEV_STATS_NUM_DCM_COUNTERS         5
#define ATH12K_HTT_TX_PDEV_STATS_NUM_BW_COUNTERS          4
#define ATH12K_HTT_TX_PDEV_STATS_NUM_SPATIAL_STREAMS      8
#define ATH12K_HTT_TX_PDEV_STATS_NUM_PREAMBLE_TYPES       7
#define ATH12K_HTT_TX_PDEV_STATS_NUM_LEGACY_CCK_STATS     4
#define ATH12K_HTT_TX_PDEV_STATS_NUM_LEGACY_OFDM_STATS    8
#define ATH12K_HTT_TX_PDEV_STATS_NUM_LTF                  4
#define ATH12K_HTT_TX_PDEV_STATS_NUM_EXTRA_MCS_COUNTERS   2
#define ATH12K_HTT_TX_PDEV_STATS_NUM_EXTRA2_MCS_COUNTERS  2
#define ATH12K_HTT_TX_PDEV_STATS_NUM_11AX_TRIGGER_TYPES   6

struct ath12k_htt_tx_pdev_rate_stats_tlv {
	uint32_t mac_id_word;
	uint32_t tx_ldpc;
	uint32_t rts_cnt;
	uint32_t ack_rssi;
	uint32_t tx_mcs[ATH12K_HTT_TX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t tx_su_mcs[ATH12K_HTT_TX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t tx_mu_mcs[ATH12K_HTT_TX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t tx_nss[ATH12K_HTT_TX_PDEV_STATS_NUM_SPATIAL_STREAMS];
	uint32_t tx_bw[ATH12K_HTT_TX_PDEV_STATS_NUM_BW_COUNTERS];
	uint32_t tx_stbc[ATH12K_HTT_TX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t tx_pream[ATH12K_HTT_TX_PDEV_STATS_NUM_PREAMBLE_TYPES];
	uint32_t tx_gi[ATH12K_HTT_TX_PDEV_STATS_NUM_GI_COUNTERS]
		[ATH12K_HTT_TX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t tx_dcm[ATH12K_HTT_TX_PDEV_STATS_NUM_DCM_COUNTERS];
	uint32_t rts_success;
	uint32_t tx_legacy_cck_rate[ATH12K_HTT_TX_PDEV_STATS_NUM_LEGACY_CCK_STATS];
	uint32_t tx_legacy_ofdm_rate[ATH12K_HTT_TX_PDEV_STATS_NUM_LEGACY_OFDM_STATS];
	uint32_t ac_mu_mimo_tx_ldpc;
	uint32_t ax_mu_mimo_tx_ldpc;
	uint32_t ofdma_tx_ldpc;
	uint32_t tx_he_ltf[ATH12K_HTT_TX_PDEV_STATS_NUM_LTF];
	uint32_t ac_mu_mimo_tx_mcs[ATH12K_HTT_TX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t ax_mu_mimo_tx_mcs[ATH12K_HTT_TX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t ofdma_tx_mcs[ATH12K_HTT_TX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t ac_mu_mimo_tx_nss[ATH12K_HTT_TX_PDEV_STATS_NUM_SPATIAL_STREAMS];
	uint32_t ax_mu_mimo_tx_nss[ATH12K_HTT_TX_PDEV_STATS_NUM_SPATIAL_STREAMS];
	uint32_t ofdma_tx_nss[ATH12K_HTT_TX_PDEV_STATS_NUM_SPATIAL_STREAMS];
	uint32_t ac_mu_mimo_tx_bw[ATH12K_HTT_TX_PDEV_STATS_NUM_BW_COUNTERS];
	uint32_t ax_mu_mimo_tx_bw[ATH12K_HTT_TX_PDEV_STATS_NUM_BW_COUNTERS];
	uint32_t ofdma_tx_bw[ATH12K_HTT_TX_PDEV_STATS_NUM_BW_COUNTERS];
	uint32_t ac_mu_mimo_tx_gi[ATH12K_HTT_TX_PDEV_STATS_NUM_GI_COUNTERS]
			    [ATH12K_HTT_TX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t ax_mimo_tx_gi[ATH12K_HTT_TX_PDEV_STATS_NUM_GI_COUNTERS]
			    [ATH12K_HTT_TX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t ofdma_tx_gi[ATH12K_HTT_TX_PDEV_STATS_NUM_GI_COUNTERS]
		       [ATH12K_HTT_TX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t trigger_type_11ax[ATH12K_HTT_TX_PDEV_STATS_NUM_11AX_TRIGGER_TYPES];
	uint32_t tx_11ax_su_ext;
	uint32_t tx_mcs_ext[ATH12K_HTT_TX_PDEV_STATS_NUM_EXTRA_MCS_COUNTERS];
	uint32_t tx_stbc_ext[ATH12K_HTT_TX_PDEV_STATS_NUM_EXTRA_MCS_COUNTERS];
	uint32_t tx_gi_ext[ATH12K_HTT_TX_PDEV_STATS_NUM_GI_COUNTERS]
		     [ATH12K_HTT_TX_PDEV_STATS_NUM_EXTRA_MCS_COUNTERS];
	uint32_t ax_mu_mimo_tx_mcs_ext[ATH12K_HTT_TX_PDEV_STATS_NUM_EXTRA_MCS_COUNTERS];
	uint32_t ofdma_tx_mcs_ext[ATH12K_HTT_TX_PDEV_STATS_NUM_EXTRA_MCS_COUNTERS];
	uint32_t ax_tx_gi_ext[ATH12K_HTT_TX_PDEV_STATS_NUM_GI_COUNTERS]
				[ATH12K_HTT_TX_PDEV_STATS_NUM_EXTRA_MCS_COUNTERS];
	uint32_t ofd_tx_gi_ext[ATH12K_HTT_TX_PDEV_STATS_NUM_GI_COUNTERS]
			   [ATH12K_HTT_TX_PDEV_STATS_NUM_EXTRA_MCS_COUNTERS];
	uint32_t tx_mcs_ext_2[ATH12K_HTT_TX_PDEV_STATS_NUM_EXTRA2_MCS_COUNTERS];
	uint32_t tx_bw_320mhz;
};

#define ATH12K_HTT_RX_PDEV_STATS_NUM_LEGACY_CCK_STATS		4
#define ATH12K_HTT_RX_PDEV_STATS_NUM_LEGACY_OFDM_STATS		8
#define ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS		12
#define ATH12K_HTT_RX_PDEV_STATS_NUM_GI_COUNTERS		4
#define ATH12K_HTT_RX_PDEV_STATS_NUM_DCM_COUNTERS		5
#define ATH12K_HTT_RX_PDEV_STATS_NUM_BW_COUNTERS		4
#define ATH12K_HTT_RX_PDEV_STATS_NUM_SPATIAL_STREAMS		8
#define ATH12K_HTT_RX_PDEV_STATS_NUM_PREAMBLE_TYPES		7
#define ATH12K_HTT_RX_PDEV_MAX_OFDMA_NUM_USER			8
#define ATH12K_HTT_RX_PDEV_STATS_RXEVM_MAX_PILOTS_NSS		16
#define ATH12K_HTT_RX_PDEV_STATS_NUM_RU_SIZE_COUNTERS		6
#define ATH12K_HTT_RX_PDEV_MAX_ULMUMIMO_NUM_USER		8
#define ATH12K_HTT_RX_PDEV_STATS_NUM_EXTRA_MCS_COUNTERS		2

struct ath12k_htt_rx_pdev_rate_stats_tlv {
	uint32_t mac_id_word;
	uint32_t nsts;
	uint32_t rx_ldpc;
	uint32_t rts_cnt;
	uint32_t rssi_mgmt;
	uint32_t rssi_data;
	uint32_t rssi_comb;
	uint32_t rx_mcs[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t rx_nss[ATH12K_HTT_RX_PDEV_STATS_NUM_SPATIAL_STREAMS];
	uint32_t rx_dcm[ATH12K_HTT_RX_PDEV_STATS_NUM_DCM_COUNTERS];
	uint32_t rx_stbc[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t rx_bw[ATH12K_HTT_RX_PDEV_STATS_NUM_BW_COUNTERS];
	uint32_t rx_pream[ATH12K_HTT_RX_PDEV_STATS_NUM_PREAMBLE_TYPES];
	uint8_t rssi_chain_in_db[ATH12K_HTT_RX_PDEV_STATS_NUM_SPATIAL_STREAMS]
		     [ATH12K_HTT_RX_PDEV_STATS_NUM_BW_COUNTERS];
	uint32_t rx_gi[ATH12K_HTT_RX_PDEV_STATS_NUM_GI_COUNTERS]
		[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t rssi_in_dbm;
	uint32_t rx_11ax_su_ext;
	uint32_t rx_11ac_mumimo;
	uint32_t rx_11ax_mumimo;
	uint32_t rx_11ax_ofdma;
	uint32_t txbf;
	uint32_t rx_legacy_cck_rate[ATH12K_HTT_RX_PDEV_STATS_NUM_LEGACY_CCK_STATS];
	uint32_t rx_legacy_ofdm_rate[ATH12K_HTT_RX_PDEV_STATS_NUM_LEGACY_OFDM_STATS];
	uint32_t rx_active_dur_us_low;
	uint32_t rx_active_dur_us_high;
	uint32_t rx_11ax_ul_ofdma;
	uint32_t ul_ofdma_rx_mcs[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t ul_ofdma_rx_gi[ATH12K_HTT_TX_PDEV_STATS_NUM_GI_COUNTERS]
			  [ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t ul_ofdma_rx_nss[ATH12K_HTT_TX_PDEV_STATS_NUM_SPATIAL_STREAMS];
	uint32_t ul_ofdma_rx_bw[ATH12K_HTT_TX_PDEV_STATS_NUM_BW_COUNTERS];
	uint32_t ul_ofdma_rx_stbc;
	uint32_t ul_ofdma_rx_ldpc;
	uint32_t rx_ulofdma_non_data_ppdu[ATH12K_HTT_RX_PDEV_MAX_OFDMA_NUM_USER];
	uint32_t rx_ulofdma_data_ppdu[ATH12K_HTT_RX_PDEV_MAX_OFDMA_NUM_USER];
	uint32_t rx_ulofdma_mpdu_ok[ATH12K_HTT_RX_PDEV_MAX_OFDMA_NUM_USER];
	uint32_t rx_ulofdma_mpdu_fail[ATH12K_HTT_RX_PDEV_MAX_OFDMA_NUM_USER];
	uint32_t nss_count;
	uint32_t pilot_count;
	uint32_t rx_pil_evm_db[ATH12K_HTT_RX_PDEV_STATS_NUM_SPATIAL_STREAMS]
			   [ATH12K_HTT_RX_PDEV_STATS_RXEVM_MAX_PILOTS_NSS];
	uint32_t rx_pilot_evm_db_mean[ATH12K_HTT_RX_PDEV_STATS_NUM_SPATIAL_STREAMS];
	int8_t rx_ul_fd_rssi[ATH12K_HTT_RX_PDEV_STATS_NUM_SPATIAL_STREAMS]
			[ATH12K_HTT_RX_PDEV_MAX_OFDMA_NUM_USER];
	uint32_t per_chain_rssi_pkt_type;
	int8_t rx_per_chain_rssi_in_dbm[ATH12K_HTT_RX_PDEV_STATS_NUM_SPATIAL_STREAMS]
				   [ATH12K_HTT_RX_PDEV_STATS_NUM_BW_COUNTERS];
	uint32_t rx_su_ndpa;
	uint32_t rx_11ax_su_txbf_mcs[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t rx_mu_ndpa;
	uint32_t rx_11ax_mu_txbf_mcs[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t rx_br_poll;
	uint32_t rx_11ax_dl_ofdma_mcs[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS];
	uint32_t rx_11ax_dl_ofdma_ru[ATH12K_HTT_RX_PDEV_STATS_NUM_RU_SIZE_COUNTERS];
	uint32_t rx_ulmumimo_non_data_ppdu[ATH12K_HTT_RX_PDEV_MAX_ULMUMIMO_NUM_USER];
	uint32_t rx_ulmumimo_data_ppdu[ATH12K_HTT_RX_PDEV_MAX_ULMUMIMO_NUM_USER];
	uint32_t rx_ulmumimo_mpdu_ok[ATH12K_HTT_RX_PDEV_MAX_ULMUMIMO_NUM_USER];
	uint32_t rx_ulmumimo_mpdu_fail[ATH12K_HTT_RX_PDEV_MAX_ULMUMIMO_NUM_USER];
	uint32_t rx_ulofdma_non_data_nusers[ATH12K_HTT_RX_PDEV_MAX_OFDMA_NUM_USER];
	uint32_t rx_ulofdma_data_nusers[ATH12K_HTT_RX_PDEV_MAX_OFDMA_NUM_USER];
	uint32_t rx_mcs_ext[ATH12K_HTT_RX_PDEV_STATS_NUM_EXTRA_MCS_COUNTERS];
};

#define ATH12K_HTT_RX_PDEV_STATS_NUM_BW_EXT_COUNTERS		4
#define ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS_EXT		14
#define ATH12K_HTT_RX_PDEV_STATS_NUM_EXTRA2_MCS_COUNTERS	2
#define ATH12K_HTT_RX_PDEV_STATS_NUM_BW_EXT2_COUNTERS		5
#define ATH12K_HTT_RX_PDEV_STATS_NUM_PUNCTURED_MODE_COUNTERS	5

struct ath12k_htt_rx_pdev_rate_ext_stats_tlv {
	uint8_t rssi_chain_ext[ATH12K_HTT_RX_PDEV_STATS_NUM_SPATIAL_STREAMS]
			 [ATH12K_HTT_RX_PDEV_STATS_NUM_BW_EXT_COUNTERS];
	int8_t rx_per_chain_rssi_ext_in_dbm[ATH12K_HTT_RX_PDEV_STATS_NUM_SPATIAL_STREAMS]
				       [ATH12K_HTT_RX_PDEV_STATS_NUM_BW_EXT_COUNTERS];
	uint32_t rssi_mcast_in_dbm;
	uint32_t rssi_mgmt_in_dbm;
	uint32_t rx_mcs_ext[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS_EXT];
	uint32_t rx_stbc_ext[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS_EXT];
	uint32_t rx_gi_ext[ATH12K_HTT_RX_PDEV_STATS_NUM_GI_COUNTERS]
		     [ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS_EXT];
	uint32_t ul_ofdma_rx_mcs_ext[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS_EXT];
	uint32_t ul_ofdma_rx_gi_ext[ATH12K_HTT_TX_PDEV_STATS_NUM_GI_COUNTERS]
			      [ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS_EXT];
	uint32_t rx_11ax_su_txbf_mcs_ext[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS_EXT];
	uint32_t rx_11ax_mu_txbf_mcs_ext[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS_EXT];
	uint32_t rx_11ax_dl_ofdma_mcs_ext[ATH12K_HTT_RX_PDEV_STATS_NUM_MCS_COUNTERS_EXT];
	uint32_t rx_mcs_ext_2[ATH12K_HTT_RX_PDEV_STATS_NUM_EXTRA2_MCS_COUNTERS];
	uint32_t rx_bw_ext[ATH12K_HTT_RX_PDEV_STATS_NUM_BW_EXT2_COUNTERS];
	uint32_t rx_gi_ext_2[ATH12K_HTT_RX_PDEV_STATS_NUM_GI_COUNTERS]
		[ATH12K_HTT_RX_PDEV_STATS_NUM_EXTRA2_MCS_COUNTERS];
	uint32_t rx_su_punctured_mode[ATH12K_HTT_RX_PDEV_STATS_NUM_PUNCTURED_MODE_COUNTERS];
};

#define ATH12K_HTT_TX_PDEV_STATS_SCHED_PER_TXQ_MAC_ID	GENMASK(7, 0)
#define ATH12K_HTT_TX_PDEV_STATS_SCHED_PER_TXQ_ID	GENMASK(15, 8)

#define ATH12K_HTT_TX_PDEV_NUM_SCHED_ORDER_LOG	20

struct ath12k_htt_stats_tx_sched_cmn_tlv {
	uint32_t mac_id__word;
	uint32_t current_timestamp;
} __packed;

struct ath12k_htt_tx_pdev_stats_sched_per_txq_tlv {
	uint32_t mac_id__word;
	uint32_t sched_policy;
	uint32_t last_sched_cmd_posted_timestamp;
	uint32_t last_sched_cmd_compl_timestamp;
	uint32_t sched_2_tac_lwm_count;
	uint32_t sched_2_tac_ring_full;
	uint32_t sched_cmd_post_failure;
	uint32_t num_active_tids;
	uint32_t num_ps_schedules;
	uint32_t sched_cmds_pending;
	uint32_t num_tid_register;
	uint32_t num_tid_unregister;
	uint32_t num_qstats_queried;
	uint32_t qstats_update_pending;
	uint32_t last_qstats_query_timestamp;
	uint32_t num_tqm_cmdq_full;
	uint32_t num_de_sched_algo_trigger;
	uint32_t num_rt_sched_algo_trigger;
	uint32_t num_tqm_sched_algo_trigger;
	uint32_t notify_sched;
	uint32_t dur_based_sendn_term;
	uint32_t su_notify2_sched;
	uint32_t su_optimal_queued_msdus_sched;
	uint32_t su_delay_timeout_sched;
	uint32_t su_min_txtime_sched_delay;
	uint32_t su_no_delay;
	uint32_t num_supercycles;
	uint32_t num_subcycles_with_sort;
	uint32_t num_subcycles_no_sort;
} __packed;

struct ath12k_htt_sched_txq_cmd_posted_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, sched_cmd_posted);
} __packed;

struct ath12k_htt_sched_txq_cmd_reaped_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, sched_cmd_reaped);
} __packed;

struct ath12k_htt_sched_txq_sched_order_su_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, sched_order_su);
} __packed;

struct ath12k_htt_sched_txq_sched_ineligibility_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, sched_ineligibility);
} __packed;

enum ath12k_htt_sched_txq_supercycle_triggers_tlv_enum {
	ATH12K_HTT_SCHED_SUPERCYCLE_TRIGGER_NONE = 0,
	ATH12K_HTT_SCHED_SUPERCYCLE_TRIGGER_FORCED,
	ATH12K_HTT_SCHED_SUPERCYCLE_TRIGGER_LESS_NUM_TIDQ_ENTRIES,
	ATH12K_HTT_SCHED_SUPERCYCLE_TRIGGER_LESS_NUM_ACTIVE_TIDS,
	ATH12K_HTT_SCHED_SUPERCYCLE_TRIGGER_MAX_ITR_REACHED,
	ATH12K_HTT_SCHED_SUPERCYCLE_TRIGGER_DUR_THRESHOLD_REACHED,
	ATH12K_HTT_SCHED_SUPERCYCLE_TRIGGER_TWT_TRIGGER,
	ATH12K_HTT_SCHED_SUPERCYCLE_TRIGGER_MAX,
};

struct ath12k_htt_sched_txq_supercycle_triggers_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, supercycle_triggers);
} __packed;

struct ath12k_htt_hw_stats_pdev_errs_tlv {
	uint32_t mac_id__word;
	uint32_t tx_abort;
	uint32_t tx_abort_fail_count;
	uint32_t rx_abort;
	uint32_t rx_abort_fail_count;
	uint32_t warm_reset;
	uint32_t cold_reset;
	uint32_t tx_flush;
	uint32_t tx_glb_reset;
	uint32_t tx_txq_reset;
	uint32_t rx_timeout_reset;
	uint32_t mac_cold_reset_restore_cal;
	uint32_t mac_cold_reset;
	uint32_t mac_warm_reset;
	uint32_t mac_only_reset;
	uint32_t phy_warm_reset;
	uint32_t phy_warm_reset_ucode_trig;
	uint32_t mac_warm_reset_restore_cal;
	uint32_t mac_sfm_reset;
	uint32_t phy_warm_reset_m3_ssr;
	uint32_t phy_warm_reset_reason_phy_m3;
	uint32_t phy_warm_reset_reason_tx_hw_stuck;
	uint32_t phy_warm_reset_reason_num_rx_frame_stuck;
	uint32_t phy_warm_reset_reason_wal_rx_rec_rx_busy;
	uint32_t phy_warm_reset_reason_wal_rx_rec_mac_hng;
	uint32_t phy_warm_reset_reason_mac_conv_phy_reset;
	uint32_t wal_rx_recovery_rst_mac_hang_cnt;
	uint32_t wal_rx_recovery_rst_known_sig_cnt;
	uint32_t wal_rx_recovery_rst_no_rx_cnt;
	uint32_t wal_rx_recovery_rst_no_rx_consec_cnt;
	uint32_t wal_rx_recovery_rst_rx_busy_cnt;
	uint32_t wal_rx_recovery_rst_phy_mac_hang_cnt;
	uint32_t rx_flush_cnt;
	uint32_t phy_warm_reset_reason_tx_exp_cca_stuck;
	uint32_t phy_warm_reset_reason_tx_consec_flsh_war;
	uint32_t phy_warm_reset_reason_tx_hwsch_reset_war;
	uint32_t phy_warm_reset_reason_hwsch_cca_wdog_war;
	uint32_t fw_rx_rings_reset;
	uint32_t rx_dest_drain_rx_descs_leak_prevented;
	uint32_t rx_dest_drain_rx_descs_saved_cnt;
	uint32_t rx_dest_drain_rxdma2reo_leak_detected;
	uint32_t rx_dest_drain_rxdma2fw_leak_detected;
	uint32_t rx_dest_drain_rxdma2wbm_leak_detected;
	uint32_t rx_dest_drain_rxdma1_2sw_leak_detected;
	uint32_t rx_dest_drain_rx_drain_ok_mac_idle;
	uint32_t rx_dest_drain_ok_mac_not_idle;
	uint32_t rx_dest_drain_prerequisite_invld;
	uint32_t rx_dest_drain_skip_non_lmac_reset;
	uint32_t rx_dest_drain_hw_fifo_notempty_post_wait;
} __packed;

#define ATH12K_HTT_STATS_MAX_HW_INTR_NAME_LEN 8
struct ath12k_htt_hw_stats_intr_misc_tlv {
	uint8_t hw_intr_name[ATH12K_HTT_STATS_MAX_HW_INTR_NAME_LEN];
	uint32_t mask;
	uint32_t count;
} __packed;

struct ath12k_htt_hw_stats_whal_tx_tlv {
	uint32_t mac_id__word;
	uint32_t last_unpause_ppdu_id;
	uint32_t hwsch_unpause_wait_tqm_write;
	uint32_t hwsch_dummy_tlv_skipped;
	uint32_t hwsch_misaligned_offset_received;
	uint32_t hwsch_reset_count;
	uint32_t hwsch_dev_reset_war;
	uint32_t hwsch_delayed_pause;
	uint32_t hwsch_long_delayed_pause;
	uint32_t sch_rx_ppdu_no_response;
	uint32_t sch_selfgen_response;
	uint32_t sch_rx_sifs_resp_trigger;
} __packed;

struct ath12k_htt_hw_war_stats_tlv {
	uint32_t mac_id__word;
	DECLARE_FLEX_ARRAY(uint32_t, hw_wars);
} __packed;

struct ath12k_htt_tx_tqm_cmn_stats_tlv {
	uint32_t mac_id__word;
	uint32_t max_cmdq_id;
	uint32_t list_mpdu_cnt_hist_intvl;
	uint32_t add_msdu;
	uint32_t q_empty;
	uint32_t q_not_empty;
	uint32_t drop_notification;
	uint32_t desc_threshold;
	uint32_t hwsch_tqm_invalid_status;
	uint32_t missed_tqm_gen_mpdus;
	uint32_t tqm_active_tids;
	uint32_t tqm_inactive_tids;
	uint32_t tqm_active_msduq_flows;
	uint32_t msduq_timestamp_updates;
	uint32_t msduq_updates_mpdu_head_info_cmd;
	uint32_t msduq_updates_emp_to_nonemp_status;
	uint32_t get_mpdu_head_info_cmds_by_query;
	uint32_t get_mpdu_head_info_cmds_by_tac;
	uint32_t gen_mpdu_cmds_by_query;
	uint32_t high_prio_q_not_empty;
} __packed;

struct ath12k_htt_tx_tqm_error_stats_tlv {
	uint32_t q_empty_failure;
	uint32_t q_not_empty_failure;
	uint32_t add_msdu_failure;
	uint32_t tqm_cache_ctl_err;
	uint32_t tqm_soft_reset;
	uint32_t tqm_reset_num_in_use_link_descs;
	uint32_t tqm_reset_num_lost_link_descs;
	uint32_t tqm_reset_num_lost_host_tx_buf_cnt;
	uint32_t tqm_reset_num_in_use_internal_tqm;
	uint32_t tqm_reset_num_in_use_idle_link_rng;
	uint32_t tqm_reset_time_to_tqm_hang_delta_ms;
	uint32_t tqm_reset_recovery_time_ms;
	uint32_t tqm_reset_num_peers_hdl;
	uint32_t tqm_reset_cumm_dirty_hw_mpduq_cnt;
	uint32_t tqm_reset_cumm_dirty_hw_msduq_proc;
	uint32_t tqm_reset_flush_cache_cmd_su_cnt;
	uint32_t tqm_reset_flush_cache_cmd_other_cnt;
	uint32_t tqm_reset_flush_cache_cmd_trig_type;
	uint32_t tqm_reset_flush_cache_cmd_trig_cfg;
	uint32_t tqm_reset_flush_cmd_skp_status_null;
} __packed;

struct ath12k_htt_tx_tqm_gen_mpdu_stats_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, gen_mpdu_end_reason);
} __packed;

#define ATH12K_HTT_TX_TQM_MAX_LIST_MPDU_END_REASON		16
#define ATH12K_HTT_TX_TQM_MAX_LIST_MPDU_CNT_HISTOGRAM_BINS	16

struct ath12k_htt_tx_tqm_list_mpdu_stats_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, list_mpdu_end_reason);
} __packed;

struct ath12k_htt_tx_tqm_list_mpdu_cnt_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, list_mpdu_cnt_hist);
} __packed;

struct ath12k_htt_tx_tqm_pdev_stats_tlv {
	uint32_t msdu_count;
	uint32_t mpdu_count;
	uint32_t remove_msdu;
	uint32_t remove_mpdu;
	uint32_t remove_msdu_ttl;
	uint32_t send_bar;
	uint32_t bar_sync;
	uint32_t notify_mpdu;
	uint32_t sync_cmd;
	uint32_t write_cmd;
	uint32_t hwsch_trigger;
	uint32_t ack_tlv_proc;
	uint32_t gen_mpdu_cmd;
	uint32_t gen_list_cmd;
	uint32_t remove_mpdu_cmd;
	uint32_t remove_mpdu_tried_cmd;
	uint32_t mpdu_queue_stats_cmd;
	uint32_t mpdu_head_info_cmd;
	uint32_t msdu_flow_stats_cmd;
	uint32_t remove_msdu_cmd;
	uint32_t remove_msdu_ttl_cmd;
	uint32_t flush_cache_cmd;
	uint32_t update_mpduq_cmd;
	uint32_t enqueue;
	uint32_t enqueue_notify;
	uint32_t notify_mpdu_at_head;
	uint32_t notify_mpdu_state_valid;
	uint32_t sched_udp_notify1;
	uint32_t sched_udp_notify2;
	uint32_t sched_nonudp_notify1;
	uint32_t sched_nonudp_notify2;
} __packed;

struct ath12k_htt_tx_de_cmn_stats_tlv {
	uint32_t mac_id__word;
	uint32_t tcl2fw_entry_count;
	uint32_t not_to_fw;
	uint32_t invalid_pdev_vdev_peer;
	uint32_t tcl_res_invalid_addrx;
	uint32_t wbm2fw_entry_count;
	uint32_t invalid_pdev;
	uint32_t tcl_res_addrx_timeout;
	uint32_t invalid_vdev;
	uint32_t invalid_tcl_exp_frame_desc;
	uint32_t vdev_id_mismatch_cnt;
} __packed;

struct ath12k_htt_tx_de_eapol_packets_stats_tlv {
	uint32_t m1_packets;
	uint32_t m2_packets;
	uint32_t m3_packets;
	uint32_t m4_packets;
	uint32_t g1_packets;
	uint32_t g2_packets;
	uint32_t rc4_packets;
	uint32_t eap_packets;
	uint32_t eapol_start_packets;
	uint32_t eapol_logoff_packets;
	uint32_t eapol_encap_asf_packets;
} __packed;

struct ath12k_htt_tx_de_classify_stats_tlv {
	uint32_t arp_packets;
	uint32_t igmp_packets;
	uint32_t dhcp_packets;
	uint32_t host_inspected;
	uint32_t htt_included;
	uint32_t htt_valid_mcs;
	uint32_t htt_valid_nss;
	uint32_t htt_valid_preamble_type;
	uint32_t htt_valid_chainmask;
	uint32_t htt_valid_guard_interval;
	uint32_t htt_valid_retries;
	uint32_t htt_valid_bw_info;
	uint32_t htt_valid_power;
	uint32_t htt_valid_key_flags;
	uint32_t htt_valid_no_encryption;
	uint32_t fse_entry_count;
	uint32_t fse_priority_be;
	uint32_t fse_priority_high;
	uint32_t fse_priority_low;
	uint32_t fse_traffic_ptrn_be;
	uint32_t fse_traffic_ptrn_over_sub;
	uint32_t fse_traffic_ptrn_bursty;
	uint32_t fse_traffic_ptrn_interactive;
	uint32_t fse_traffic_ptrn_periodic;
	uint32_t fse_hwqueue_alloc;
	uint32_t fse_hwqueue_created;
	uint32_t fse_hwqueue_send_to_host;
	uint32_t mcast_entry;
	uint32_t bcast_entry;
	uint32_t htt_update_peer_cache;
	uint32_t htt_learning_frame;
	uint32_t fse_invalid_peer;
	uint32_t mec_notify;
} __packed;

struct ath12k_htt_tx_de_classify_failed_stats_tlv {
	uint32_t ap_bss_peer_not_found;
	uint32_t ap_bcast_mcast_no_peer;
	uint32_t sta_delete_in_progress;
	uint32_t ibss_no_bss_peer;
	uint32_t invalid_vdev_type;
	uint32_t invalid_ast_peer_entry;
	uint32_t peer_entry_invalid;
	uint32_t ethertype_not_ip;
	uint32_t eapol_lookup_failed;
	uint32_t qpeer_not_allow_data;
	uint32_t fse_tid_override;
	uint32_t ipv6_jumbogram_zero_length;
	uint32_t qos_to_non_qos_in_prog;
	uint32_t ap_bcast_mcast_eapol;
	uint32_t unicast_on_ap_bss_peer;
	uint32_t ap_vdev_invalid;
	uint32_t incomplete_llc;
	uint32_t eapol_duplicate_m3;
	uint32_t eapol_duplicate_m4;
} __packed;

struct ath12k_htt_tx_de_classify_status_stats_tlv {
	uint32_t eok;
	uint32_t classify_done;
	uint32_t lookup_failed;
	uint32_t send_host_dhcp;
	uint32_t send_host_mcast;
	uint32_t send_host_unknown_dest;
	uint32_t send_host;
	uint32_t status_invalid;
} __packed;

struct ath12k_htt_tx_de_enqueue_packets_stats_tlv {
	uint32_t enqueued_pkts;
	uint32_t to_tqm;
	uint32_t to_tqm_bypass;
} __packed;

struct ath12k_htt_tx_de_enqueue_discard_stats_tlv {
	uint32_t discarded_pkts;
	uint32_t local_frames;
	uint32_t is_ext_msdu;
} __packed;

struct ath12k_htt_tx_de_compl_stats_tlv {
	uint32_t tcl_dummy_frame;
	uint32_t tqm_dummy_frame;
	uint32_t tqm_notify_frame;
	uint32_t fw2wbm_enq;
	uint32_t tqm_bypass_frame;
} __packed;

enum ath12k_htt_tx_mumimo_grp_invalid_reason_code_stats {
	ATH12K_HTT_TX_MUMIMO_GRP_VALID,
	ATH12K_HTT_TX_MUMIMO_GRP_INVALID_NUM_MU_USERS_EXCEEDED_MU_MAX_USERS,
	ATH12K_HTT_TX_MUMIMO_GRP_INVALID_SCHED_ALGO_NOT_MU_COMPATIBLE_GID,
	ATH12K_HTT_TX_MUMIMO_GRP_INVALID_NON_PRIMARY_GRP,
	ATH12K_HTT_TX_MUMIMO_GRP_INVALID_ZERO_CANDIDATES,
	ATH12K_HTT_TX_MUMIMO_GRP_INVALID_MORE_CANDIDATES,
	ATH12K_HTT_TX_MUMIMO_GRP_INVALID_GROUP_SIZE_EXCEED_NSS,
	ATH12K_HTT_TX_MUMIMO_GRP_INVALID_GROUP_INELIGIBLE,
	ATH12K_HTT_TX_MUMIMO_GRP_INVALID,
	ATH12K_HTT_TX_MUMIMO_GRP_INVALID_GROUP_EFF_MU_TPUT_OMBPS,
	ATH12K_HTT_TX_MUMIMO_GRP_INVALID_MAX_REASON_CODE,
};

#define ATH12K_HTT_NUM_AC_WMM				0x4
#define ATH12K_HTT_MAX_NUM_SBT_INTR			4
#define ATH12K_HTT_TX_NUM_AC_MUMIMO_USER_STATS		4
#define ATH12K_HTT_TX_NUM_AX_MUMIMO_USER_STATS		8
#define ATH12K_HTT_TX_NUM_BE_MUMIMO_USER_STATS		8
#define ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS	7
#define ATH12K_HTT_TX_NUM_OFDMA_USER_STATS		74
#define ATH12K_HTT_TX_NUM_UL_MUMIMO_USER_STATS		8
#define ATH12K_HTT_STATS_NUM_MAX_MUMIMO_SZ		8
#define ATH12K_HTT_STATS_MUMIMO_TPUT_NUM_BINS		10

#define ATH12K_HTT_STATS_MAX_INVALID_REASON_CODE \
	ATH12K_HTT_TX_MUMIMO_GRP_INVALID_MAX_REASON_CODE
#define ATH12K_HTT_TX_NUM_MUMIMO_GRP_INVALID_WORDS \
	(ATH12K_HTT_STATS_NUM_MAX_MUMIMO_SZ * ATH12K_HTT_STATS_MAX_INVALID_REASON_CODE)

struct ath12k_htt_tx_selfgen_cmn_stats_tlv {
	uint32_t mac_id__word;
	uint32_t su_bar;
	uint32_t rts;
	uint32_t cts2self;
	uint32_t qos_null;
	uint32_t delayed_bar_1;
	uint32_t delayed_bar_2;
	uint32_t delayed_bar_3;
	uint32_t delayed_bar_4;
	uint32_t delayed_bar_5;
	uint32_t delayed_bar_6;
	uint32_t delayed_bar_7;
} __packed;

struct ath12k_htt_tx_selfgen_ac_stats_tlv {
	uint32_t ac_su_ndpa;
	uint32_t ac_su_ndp;
	uint32_t ac_mu_mimo_ndpa;
	uint32_t ac_mu_mimo_ndp;
	uint32_t ac_mu_mimo_brpoll[ATH12K_HTT_TX_NUM_AC_MUMIMO_USER_STATS - 1];
} __packed;

struct ath12k_htt_tx_selfgen_ax_stats_tlv {
	uint32_t ax_su_ndpa;
	uint32_t ax_su_ndp;
	uint32_t ax_mu_mimo_ndpa;
	uint32_t ax_mu_mimo_ndp;
	uint32_t ax_mu_mimo_brpoll[ATH12K_HTT_TX_NUM_AX_MUMIMO_USER_STATS - 1];
	uint32_t ax_basic_trigger;
	uint32_t ax_bsr_trigger;
	uint32_t ax_mu_bar_trigger;
	uint32_t ax_mu_rts_trigger;
	uint32_t ax_ulmumimo_trigger;
} __packed;

struct ath12k_htt_tx_selfgen_be_stats_tlv {
	uint32_t be_su_ndpa;
	uint32_t be_su_ndp;
	uint32_t be_mu_mimo_ndpa;
	uint32_t be_mu_mimo_ndp;
	uint32_t be_mu_mimo_brpoll[ATH12K_HTT_TX_NUM_BE_MUMIMO_USER_STATS - 1];
	uint32_t be_basic_trigger;
	uint32_t be_bsr_trigger;
	uint32_t be_mu_bar_trigger;
	uint32_t be_mu_rts_trigger;
	uint32_t be_ulmumimo_trigger;
	uint32_t be_su_ndpa_queued;
	uint32_t be_su_ndp_queued;
	uint32_t be_mu_mimo_ndpa_queued;
	uint32_t be_mu_mimo_ndp_queued;
	uint32_t be_mu_mimo_brpoll_queued[ATH12K_HTT_TX_NUM_BE_MUMIMO_USER_STATS - 1];
	uint32_t be_ul_mumimo_trigger[ATH12K_HTT_TX_NUM_BE_MUMIMO_USER_STATS];
} __packed;

struct ath12k_htt_tx_selfgen_ac_err_stats_tlv {
	uint32_t ac_su_ndp_err;
	uint32_t ac_su_ndpa_err;
	uint32_t ac_mu_mimo_ndpa_err;
	uint32_t ac_mu_mimo_ndp_err;
	uint32_t ac_mu_mimo_brp1_err;
	uint32_t ac_mu_mimo_brp2_err;
	uint32_t ac_mu_mimo_brp3_err;
} __packed;

struct ath12k_htt_tx_selfgen_ax_err_stats_tlv {
	uint32_t ax_su_ndp_err;
	uint32_t ax_su_ndpa_err;
	uint32_t ax_mu_mimo_ndpa_err;
	uint32_t ax_mu_mimo_ndp_err;
	uint32_t ax_mu_mimo_brp_err[ATH12K_HTT_TX_NUM_AX_MUMIMO_USER_STATS - 1];
	uint32_t ax_basic_trigger_err;
	uint32_t ax_bsr_trigger_err;
	uint32_t ax_mu_bar_trigger_err;
	uint32_t ax_mu_rts_trigger_err;
	uint32_t ax_ulmumimo_trigger_err;
} __packed;

struct ath12k_htt_tx_selfgen_be_err_stats_tlv {
	uint32_t be_su_ndp_err;
	uint32_t be_su_ndpa_err;
	uint32_t be_mu_mimo_ndpa_err;
	uint32_t be_mu_mimo_ndp_err;
	uint32_t be_mu_mimo_brp_err[ATH12K_HTT_TX_NUM_BE_MUMIMO_USER_STATS - 1];
	uint32_t be_basic_trigger_err;
	uint32_t be_bsr_trigger_err;
	uint32_t be_mu_bar_trigger_err;
	uint32_t be_mu_rts_trigger_err;
	uint32_t be_ulmumimo_trigger_err;
	uint32_t be_mu_mimo_brp_err_num_cbf_rxd[ATH12K_HTT_TX_NUM_BE_MUMIMO_USER_STATS];
	uint32_t be_su_ndpa_flushed;
	uint32_t be_su_ndp_flushed;
	uint32_t be_mu_mimo_ndpa_flushed;
	uint32_t be_mu_mimo_ndp_flushed;
	uint32_t be_mu_mimo_brpoll_flushed[ATH12K_HTT_TX_NUM_BE_MUMIMO_USER_STATS - 1];
	uint32_t be_ul_mumimo_trigger_err[ATH12K_HTT_TX_NUM_BE_MUMIMO_USER_STATS];
} __packed;

enum ath12k_htt_tx_selfgen_sch_tsflag_error_stats {
	ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_FLUSH_RCVD_ERR,
	ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_FILT_SCHED_CMD_ERR,
	ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_RESP_MISMATCH_ERR,
	ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_RESP_CBF_MIMO_CTRL_MISMATCH_ERR,
	ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_RESP_CBF_BW_MISMATCH_ERR,
	ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_RETRY_COUNT_FAIL_ERR,
	ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_RESP_TOO_LATE_RECEIVED_ERR,
	ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_SIFS_STALL_NO_NEXT_CMD_ERR,

	ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS
};

struct ath12k_htt_tx_selfgen_ac_sched_status_stats_tlv {
	uint32_t ac_su_ndpa_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ac_su_ndp_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ac_su_ndp_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
	uint32_t ac_mu_mimo_ndpa_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ac_mu_mimo_ndp_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ac_mu_mimo_ndp_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
	uint32_t ac_mu_mimo_brp_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ac_mu_mimo_brp_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
} __packed;

struct ath12k_htt_tx_selfgen_ax_sched_status_stats_tlv {
	uint32_t ax_su_ndpa_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ax_su_ndp_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ax_su_ndp_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
	uint32_t ax_mu_mimo_ndpa_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ax_mu_mimo_ndp_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ax_mu_mimo_ndp_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
	uint32_t ax_mu_brp_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ax_mu_brp_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
	uint32_t ax_mu_bar_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ax_mu_bar_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
	uint32_t ax_basic_trig_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ax_basic_trig_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
	uint32_t ax_ulmumimo_trig_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t ax_ulmumimo_trig_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
} __packed;

struct ath12k_htt_tx_selfgen_be_sched_status_stats_tlv {
	uint32_t be_su_ndpa_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t be_su_ndp_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t be_su_ndp_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
	uint32_t be_mu_mimo_ndpa_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t be_mu_mimo_ndp_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t be_mu_mimo_ndp_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
	uint32_t be_mu_brp_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t be_mu_brp_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
	uint32_t be_mu_bar_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t be_mu_bar_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
	uint32_t be_basic_trig_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t be_basic_trig_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
	uint32_t be_ulmumimo_trig_sch_status[ATH12K_HTT_TX_PDEV_STATS_NUM_TX_ERR_STATUS];
	uint32_t be_ulmumimo_trig_sch_flag_err[ATH12K_HTT_TX_SELFGEN_SCH_TSFLAG_ERR_STATS];
} __packed;

struct ath12k_htt_stats_string_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, data);
} __packed;

#define ATH12K_HTT_SRING_STATS_MAC_ID                  GENMASK(7, 0)
#define ATH12K_HTT_SRING_STATS_RING_ID                 GENMASK(15, 8)
#define ATH12K_HTT_SRING_STATS_ARENA                   GENMASK(23, 16)
#define ATH12K_HTT_SRING_STATS_EP                      BIT(24)
#define ATH12K_HTT_SRING_STATS_NUM_AVAIL_WORDS         GENMASK(15, 0)
#define ATH12K_HTT_SRING_STATS_NUM_VALID_WORDS         GENMASK(31, 16)
#define ATH12K_HTT_SRING_STATS_HEAD_PTR                GENMASK(15, 0)
#define ATH12K_HTT_SRING_STATS_TAIL_PTR                GENMASK(31, 16)
#define ATH12K_HTT_SRING_STATS_CONSUMER_EMPTY          GENMASK(15, 0)
#define ATH12K_HTT_SRING_STATS_PRODUCER_FULL           GENMASK(31, 16)
#define ATH12K_HTT_SRING_STATS_PREFETCH_COUNT          GENMASK(15, 0)
#define ATH12K_HTT_SRING_STATS_INTERNAL_TAIL_PTR       GENMASK(31, 16)

struct ath12k_htt_sring_stats_tlv {
	uint32_t mac_id__ring_id__arena__ep;
	uint32_t base_addr_lsb;
	uint32_t base_addr_msb;
	uint32_t ring_size;
	uint32_t elem_size;
	uint32_t num_avail_words__num_valid_words;
	uint32_t head_ptr__tail_ptr;
	uint32_t consumer_empty__producer_full;
	uint32_t prefetch_count__internal_tail_ptr;
} __packed;

struct ath12k_htt_sfm_cmn_tlv {
	uint32_t mac_id__word;
	uint32_t buf_total;
	uint32_t mem_empty;
	uint32_t deallocate_bufs;
	uint32_t num_records;
} __packed;

struct ath12k_htt_sfm_client_tlv {
	uint32_t client_id;
	uint32_t buf_min;
	uint32_t buf_max;
	uint32_t buf_busy;
	uint32_t buf_alloc;
	uint32_t buf_avail;
	uint32_t num_users;
} __packed;

struct ath12k_htt_sfm_client_user_tlv {
	DECLARE_FLEX_ARRAY(uint32_t, dwords_used_by_user_n);
} __packed;

struct ath12k_htt_tx_pdev_mu_mimo_sch_stats_tlv {
	uint32_t mu_mimo_sch_posted;
	uint32_t mu_mimo_sch_failed;
	uint32_t mu_mimo_ppdu_posted;
	uint32_t ac_mu_mimo_sch_nusers[ATH12K_HTT_TX_NUM_AC_MUMIMO_USER_STATS];
	uint32_t ax_mu_mimo_sch_nusers[ATH12K_HTT_TX_NUM_AX_MUMIMO_USER_STATS];
	uint32_t ax_ofdma_sch_nusers[ATH12K_HTT_TX_NUM_OFDMA_USER_STATS];
	uint32_t ax_ul_ofdma_nusers[ATH12K_HTT_TX_NUM_OFDMA_USER_STATS];
	uint32_t ax_ul_ofdma_bsr_nusers[ATH12K_HTT_TX_NUM_OFDMA_USER_STATS];
	uint32_t ax_ul_ofdma_bar_nusers[ATH12K_HTT_TX_NUM_OFDMA_USER_STATS];
	uint32_t ax_ul_ofdma_brp_nusers[ATH12K_HTT_TX_NUM_OFDMA_USER_STATS];
	uint32_t ax_ul_mumimo_nusers[ATH12K_HTT_TX_NUM_UL_MUMIMO_USER_STATS];
	uint32_t ax_ul_mumimo_brp_nusers[ATH12K_HTT_TX_NUM_UL_MUMIMO_USER_STATS];
	uint32_t ac_mu_mimo_per_grp_sz[ATH12K_HTT_TX_NUM_AC_MUMIMO_USER_STATS];
	uint32_t ax_mu_mimo_per_grp_sz[ATH12K_HTT_TX_NUM_AX_MUMIMO_USER_STATS];
	uint32_t be_mu_mimo_sch_nusers[ATH12K_HTT_TX_NUM_BE_MUMIMO_USER_STATS];
	uint32_t be_mu_mimo_per_grp_sz[ATH12K_HTT_TX_NUM_BE_MUMIMO_USER_STATS];
	uint32_t ac_mu_mimo_grp_sz_ext[ATH12K_HTT_TX_NUM_AC_MUMIMO_USER_STATS];
} __packed;

struct ath12k_htt_tx_pdev_mumimo_grp_stats_tlv {
	uint32_t dl_mumimo_grp_best_grp_size[ATH12K_HTT_STATS_NUM_MAX_MUMIMO_SZ];
	uint32_t dl_mumimo_grp_best_num_usrs[ATH12K_HTT_TX_NUM_AX_MUMIMO_USER_STATS];
	uint32_t dl_mumimo_grp_eligible[ATH12K_HTT_STATS_NUM_MAX_MUMIMO_SZ];
	uint32_t dl_mumimo_grp_ineligible[ATH12K_HTT_STATS_NUM_MAX_MUMIMO_SZ];
	uint32_t dl_mumimo_grp_invalid[ATH12K_HTT_TX_NUM_MUMIMO_GRP_INVALID_WORDS];
	uint32_t dl_mumimo_grp_tputs[ATH12K_HTT_STATS_MUMIMO_TPUT_NUM_BINS];
	uint32_t ul_mumimo_grp_best_grp_size[ATH12K_HTT_STATS_NUM_MAX_MUMIMO_SZ];
	uint32_t ul_mumimo_grp_best_usrs[ATH12K_HTT_TX_NUM_AX_MUMIMO_USER_STATS];
	uint32_t ul_mumimo_grp_tputs[ATH12K_HTT_STATS_MUMIMO_TPUT_NUM_BINS];
} __packed;

enum ath12k_htt_stats_tx_sched_modes {
	ATH12K_HTT_STATS_TX_SCHED_MODE_MU_MIMO_AC = 0,
	ATH12K_HTT_STATS_TX_SCHED_MODE_MU_MIMO_AX,
	ATH12K_HTT_STATS_TX_SCHED_MODE_MU_OFDMA_AX,
	ATH12K_HTT_STATS_TX_SCHED_MODE_MU_OFDMA_BE,
	ATH12K_HTT_STATS_TX_SCHED_MODE_MU_MIMO_BE
};

struct ath12k_htt_tx_pdev_mpdu_stats_tlv {
	uint32_t mpdus_queued_usr;
	uint32_t mpdus_tried_usr;
	uint32_t mpdus_failed_usr;
	uint32_t mpdus_requeued_usr;
	uint32_t err_no_ba_usr;
	uint32_t mpdu_underrun_usr;
	uint32_t ampdu_underrun_usr;
	uint32_t user_index;
	uint32_t tx_sched_mode;
} __packed;

struct ath12k_htt_pdev_stats_cca_counters_tlv {
	uint32_t tx_frame_usec;
	uint32_t rx_frame_usec;
	uint32_t rx_clear_usec;
	uint32_t my_rx_frame_usec;
	uint32_t usec_cnt;
	uint32_t med_rx_idle_usec;
	uint32_t med_tx_idle_global_usec;
	uint32_t cca_obss_usec;
} __packed;

struct ath12k_htt_pdev_cca_stats_hist_v1_tlv {
	uint32_t chan_num;
	uint32_t num_records;
	uint32_t valid_cca_counters_bitmap;
	uint32_t collection_interval;
} __packed;

#define ATH12K_HTT_TX_CV_CORR_MAX_NUM_COLUMNS		8
#define ATH12K_HTT_TX_NUM_AC_MUMIMO_USER_STATS		4
#define ATH12K_HTT_TX_NUM_AX_MUMIMO_USER_STATS          8
#define ATH12K_HTT_TX_NUM_BE_MUMIMO_USER_STATS		8
#define ATH12K_HTT_TX_PDEV_STATS_NUM_BW_COUNTERS	4
#define ATH12K_HTT_TX_NUM_MCS_CNTRS			12
#define ATH12K_HTT_TX_NUM_EXTRA_MCS_CNTRS		2

#define ATH12K_HTT_TX_NUM_OF_SOUNDING_STATS_WORDS \
	(ATH12K_HTT_TX_PDEV_STATS_NUM_BW_COUNTERS * \
	 ATH12K_HTT_TX_NUM_AX_MUMIMO_USER_STATS)

enum ath12k_htt_txbf_sound_steer_modes {
	ATH12K_HTT_IMPL_STEER_STATS		= 0,
	ATH12K_HTT_EXPL_SUSIFS_STEER_STATS	= 1,
	ATH12K_HTT_EXPL_SURBO_STEER_STATS	= 2,
	ATH12K_HTT_EXPL_MUSIFS_STEER_STATS	= 3,
	ATH12K_HTT_EXPL_MURBO_STEER_STATS	= 4,
	ATH12K_HTT_TXBF_MAX_NUM_OF_MODES	= 5
};

enum ath12k_htt_stats_sounding_tx_mode {
	ATH12K_HTT_TX_AC_SOUNDING_MODE		= 0,
	ATH12K_HTT_TX_AX_SOUNDING_MODE		= 1,
	ATH12K_HTT_TX_BE_SOUNDING_MODE		= 2,
	ATH12K_HTT_TX_CMN_SOUNDING_MODE		= 3,
};

struct ath12k_htt_tx_sounding_stats_tlv {
	uint32_t tx_sounding_mode;
	uint32_t cbf_20[ATH12K_HTT_TXBF_MAX_NUM_OF_MODES];
	uint32_t cbf_40[ATH12K_HTT_TXBF_MAX_NUM_OF_MODES];
	uint32_t cbf_80[ATH12K_HTT_TXBF_MAX_NUM_OF_MODES];
	uint32_t cbf_160[ATH12K_HTT_TXBF_MAX_NUM_OF_MODES];
	uint32_t sounding[ATH12K_HTT_TX_NUM_OF_SOUNDING_STATS_WORDS];
	uint32_t cv_nc_mismatch_err;
	uint32_t cv_fcs_err;
	uint32_t cv_frag_idx_mismatch;
	uint32_t cv_invalid_peer_id;
	uint32_t cv_no_txbf_setup;
	uint32_t cv_expiry_in_update;
	uint32_t cv_pkt_bw_exceed;
	uint32_t cv_dma_not_done_err;
	uint32_t cv_update_failed;
	uint32_t cv_total_query;
	uint32_t cv_total_pattern_query;
	uint32_t cv_total_bw_query;
	uint32_t cv_invalid_bw_coding;
	uint32_t cv_forced_sounding;
	uint32_t cv_standalone_sounding;
	uint32_t cv_nc_mismatch;
	uint32_t cv_fb_type_mismatch;
	uint32_t cv_ofdma_bw_mismatch;
	uint32_t cv_bw_mismatch;
	uint32_t cv_pattern_mismatch;
	uint32_t cv_preamble_mismatch;
	uint32_t cv_nr_mismatch;
	uint32_t cv_in_use_cnt_exceeded;
	uint32_t cv_found;
	uint32_t cv_not_found;
	uint32_t sounding_320[ATH12K_HTT_TX_NUM_BE_MUMIMO_USER_STATS];
	uint32_t cbf_320[ATH12K_HTT_TXBF_MAX_NUM_OF_MODES];
	uint32_t cv_ntbr_sounding;
	uint32_t cv_found_upload_in_progress;
	uint32_t cv_expired_during_query;
	uint32_t cv_dma_timeout_error;
	uint32_t cv_buf_ibf_uploads;
	uint32_t cv_buf_ebf_uploads;
	uint32_t cv_buf_received;
	uint32_t cv_buf_fed_back;
	uint32_t cv_total_query_ibf;
	uint32_t cv_found_ibf;
	uint32_t cv_not_found_ibf;
	uint32_t cv_expired_during_query_ibf;
} __packed;

struct ath12k_htt_pdev_obss_pd_stats_tlv {
	uint32_t num_obss_tx_ppdu_success;
	uint32_t num_obss_tx_ppdu_failure;
	uint32_t num_sr_tx_transmissions;
	uint32_t num_spatial_reuse_opportunities;
	uint32_t num_non_srg_opportunities;
	uint32_t num_non_srg_ppdu_tried;
	uint32_t num_non_srg_ppdu_success;
	uint32_t num_srg_opportunities;
	uint32_t num_srg_ppdu_tried;
	uint32_t num_srg_ppdu_success;
	uint32_t num_psr_opportunities;
	uint32_t num_psr_ppdu_tried;
	uint32_t num_psr_ppdu_success;
	uint32_t num_non_srg_tried_per_ac[ATH12K_HTT_NUM_AC_WMM];
	uint32_t num_non_srg_success_ac[ATH12K_HTT_NUM_AC_WMM];
	uint32_t num_srg_tried_per_ac[ATH12K_HTT_NUM_AC_WMM];
	uint32_t num_srg_success_per_ac[ATH12K_HTT_NUM_AC_WMM];
	uint32_t num_obss_min_dur_check_flush_cnt;
	uint32_t num_sr_ppdu_abort_flush_cnt;
} __packed;

#define ATH12K_HTT_STATS_MAX_PROF_STATS_NAME_LEN	32
#define ATH12K_HTT_LATENCY_PROFILE_NUM_MAX_HIST		3
#define ATH12K_HTT_INTERRUPTS_LATENCY_PROFILE_MAX_HIST	3

struct ath12k_htt_latency_prof_stats_tlv {
	uint32_t print_header;
	int8_t latency_prof_name[ATH12K_HTT_STATS_MAX_PROF_STATS_NAME_LEN];
	uint32_t cnt;
	uint32_t min;
	uint32_t max;
	uint32_t last;
	uint32_t tot;
	uint32_t avg;
	uint32_t hist_intvl;
	uint32_t hist[ATH12K_HTT_LATENCY_PROFILE_NUM_MAX_HIST];
}  __packed;

struct ath12k_htt_latency_prof_ctx_tlv {
	uint32_t duration;
	uint32_t tx_msdu_cnt;
	uint32_t tx_mpdu_cnt;
	uint32_t tx_ppdu_cnt;
	uint32_t rx_msdu_cnt;
	uint32_t rx_mpdu_cnt;
} __packed;

struct ath12k_htt_latency_prof_cnt_tlv {
	uint32_t prof_enable_cnt;
} __packed;

#define ATH12K_HTT_RX_NUM_MCS_CNTRS		12
#define ATH12K_HTT_RX_NUM_GI_CNTRS		4
#define ATH12K_HTT_RX_NUM_SPATIAL_STREAMS	8
#define ATH12K_HTT_RX_NUM_BW_CNTRS		4
#define ATH12K_HTT_RX_NUM_RU_SIZE_CNTRS		6
#define ATH12K_HTT_RX_NUM_RU_SIZE_160MHZ_CNTRS	7
#define ATH12K_HTT_RX_UL_MAX_UPLINK_RSSI_TRACK	5
#define ATH12K_HTT_RX_NUM_REDUCED_CHAN_TYPES	2
#define ATH12K_HTT_RX_NUM_EXTRA_MCS_CNTRS	2

enum ATH12K_HTT_TX_RX_PDEV_STATS_AX_RU_SIZE {
	ATH12K_HTT_TX_RX_PDEV_STATS_AX_RU_SIZE_26,
	ATH12K_HTT_TX_RX_PDEV_STATS_AX_RU_SIZE_52,
	ATH12K_HTT_TX_RX_PDEV_STATS_AX_RU_SIZE_106,
	ATH12K_HTT_TX_RX_PDEV_STATS_AX_RU_SIZE_242,
	ATH12K_HTT_TX_RX_PDEV_STATS_AX_RU_SIZE_484,
	ATH12K_HTT_TX_RX_PDEV_STATS_AX_RU_SIZE_996,
	ATH12K_HTT_TX_RX_PDEV_STATS_AX_RU_SIZE_996x2,
	ATH12K_HTT_TX_RX_PDEV_STATS_NUM_AX_RU_SIZE_CNTRS,
};

struct ath12k_htt_rx_pdev_ul_ofdma_user_stats_tlv {
	uint32_t user_index;
	uint32_t rx_ulofdma_non_data_ppdu;
	uint32_t rx_ulofdma_data_ppdu;
	uint32_t rx_ulofdma_mpdu_ok;
	uint32_t rx_ulofdma_mpdu_fail;
	uint32_t rx_ulofdma_non_data_nusers;
	uint32_t rx_ulofdma_data_nusers;
} __packed;

struct ath12k_htt_rx_pdev_ul_trigger_stats_tlv {
	uint32_t mac_id__word;
	uint32_t rx_11ax_ul_ofdma;
	uint32_t ul_ofdma_rx_mcs[ATH12K_HTT_RX_NUM_MCS_CNTRS];
	uint32_t ul_ofdma_rx_gi[ATH12K_HTT_RX_NUM_GI_CNTRS][ATH12K_HTT_RX_NUM_MCS_CNTRS];
	uint32_t ul_ofdma_rx_nss[ATH12K_HTT_RX_NUM_SPATIAL_STREAMS];
	uint32_t ul_ofdma_rx_bw[ATH12K_HTT_RX_NUM_BW_CNTRS];
	uint32_t ul_ofdma_rx_stbc;
	uint32_t ul_ofdma_rx_ldpc;
	uint32_t data_ru_size_ppdu[ATH12K_HTT_RX_NUM_RU_SIZE_160MHZ_CNTRS];
	uint32_t non_data_ru_size_ppdu[ATH12K_HTT_RX_NUM_RU_SIZE_160MHZ_CNTRS];
	uint32_t uplink_sta_aid[ATH12K_HTT_RX_UL_MAX_UPLINK_RSSI_TRACK];
	uint32_t uplink_sta_target_rssi[ATH12K_HTT_RX_UL_MAX_UPLINK_RSSI_TRACK];
	uint32_t uplink_sta_fd_rssi[ATH12K_HTT_RX_UL_MAX_UPLINK_RSSI_TRACK];
	uint32_t uplink_sta_power_headroom[ATH12K_HTT_RX_UL_MAX_UPLINK_RSSI_TRACK];
	uint32_t red_bw[ATH12K_HTT_RX_NUM_REDUCED_CHAN_TYPES][ATH12K_HTT_RX_NUM_BW_CNTRS];
	uint32_t ul_ofdma_bsc_trig_rx_qos_null_only;
} __packed;

#define ATH12K_HTT_TX_UL_MUMIMO_USER_STATS	8

struct ath12k_htt_rx_ul_mumimo_trig_stats_tlv {
	uint32_t mac_id__word;
	uint32_t rx_11ax_ul_mumimo;
	uint32_t ul_mumimo_rx_mcs[ATH12K_HTT_RX_NUM_MCS_CNTRS];
	uint32_t ul_rx_gi[ATH12K_HTT_RX_NUM_GI_CNTRS][ATH12K_HTT_RX_NUM_MCS_CNTRS];
	uint32_t ul_mumimo_rx_nss[ATH12K_HTT_RX_NUM_SPATIAL_STREAMS];
	uint32_t ul_mumimo_rx_bw[ATH12K_HTT_RX_NUM_BW_CNTRS];
	uint32_t ul_mumimo_rx_stbc;
	uint32_t ul_mumimo_rx_ldpc;
	uint32_t ul_mumimo_rx_mcs_ext[ATH12K_HTT_RX_NUM_EXTRA_MCS_CNTRS];
	uint32_t ul_gi_ext[ATH12K_HTT_RX_NUM_GI_CNTRS][ATH12K_HTT_RX_NUM_EXTRA_MCS_CNTRS];
	int8_t ul_rssi[ATH12K_HTT_RX_NUM_SPATIAL_STREAMS][ATH12K_HTT_RX_NUM_BW_CNTRS];
	int8_t tgt_rssi[ATH12K_HTT_TX_UL_MUMIMO_USER_STATS][ATH12K_HTT_RX_NUM_BW_CNTRS];
	int8_t fd[ATH12K_HTT_TX_UL_MUMIMO_USER_STATS][ATH12K_HTT_RX_NUM_SPATIAL_STREAMS];
	int8_t db[ATH12K_HTT_TX_UL_MUMIMO_USER_STATS][ATH12K_HTT_RX_NUM_SPATIAL_STREAMS];
	uint32_t red_bw[ATH12K_HTT_RX_NUM_REDUCED_CHAN_TYPES][ATH12K_HTT_RX_NUM_BW_CNTRS];
	uint32_t mumimo_bsc_trig_rx_qos_null_only;
} __packed;

#define ATH12K_HTT_RX_NUM_MAX_PEAK_OCCUPANCY_INDEX	10
#define ATH12K_HTT_RX_NUM_MAX_CURR_OCCUPANCY_INDEX	10
#define ATH12K_HTT_RX_NUM_SQUARE_INDEX			6
#define ATH12K_HTT_RX_NUM_MAX_PEAK_SEARCH_INDEX		4
#define ATH12K_HTT_RX_NUM_MAX_PENDING_SEARCH_INDEX	4

struct ath12k_htt_rx_fse_stats_tlv {
	uint32_t fse_enable_cnt;
	uint32_t fse_disable_cnt;
	uint32_t fse_cache_invalidate_entry_cnt;
	uint32_t fse_full_cache_invalidate_cnt;
	uint32_t fse_num_cache_hits_cnt;
	uint32_t fse_num_searches_cnt;
	uint32_t fse_cache_occupancy_peak_cnt[ATH12K_HTT_RX_NUM_MAX_PEAK_OCCUPANCY_INDEX];
	uint32_t fse_cache_occupancy_curr_cnt[ATH12K_HTT_RX_NUM_MAX_CURR_OCCUPANCY_INDEX];
	uint32_t fse_search_stat_square_cnt[ATH12K_HTT_RX_NUM_SQUARE_INDEX];
	uint32_t fse_search_stat_peak_cnt[ATH12K_HTT_RX_NUM_MAX_PEAK_SEARCH_INDEX];
	uint32_t fse_search_stat_pending_cnt[ATH12K_HTT_RX_NUM_MAX_PENDING_SEARCH_INDEX];
} __packed;

#define ATH12K_HTT_TX_BF_RATE_STATS_NUM_MCS_COUNTERS		14
#define ATH12K_HTT_TX_PDEV_STATS_NUM_LEGACY_OFDM_STATS		8
#define ATH12K_HTT_TX_PDEV_STATS_NUM_SPATIAL_STREAMS		8
#define ATH12K_HTT_TXBF_NUM_BW_CNTRS				5
#define ATH12K_HTT_TXBF_NUM_REDUCED_CHAN_TYPES			2

struct ath12k_htt_pdev_txrate_txbf_stats_tlv {
	uint32_t tx_su_txbf_mcs[ATH12K_HTT_TX_BF_RATE_STATS_NUM_MCS_COUNTERS];
	uint32_t tx_su_ibf_mcs[ATH12K_HTT_TX_BF_RATE_STATS_NUM_MCS_COUNTERS];
	uint32_t tx_su_ol_mcs[ATH12K_HTT_TX_BF_RATE_STATS_NUM_MCS_COUNTERS];
	uint32_t tx_su_txbf_nss[ATH12K_HTT_TX_PDEV_STATS_NUM_SPATIAL_STREAMS];
	uint32_t tx_su_ibf_nss[ATH12K_HTT_TX_PDEV_STATS_NUM_SPATIAL_STREAMS];
	uint32_t tx_su_ol_nss[ATH12K_HTT_TX_PDEV_STATS_NUM_SPATIAL_STREAMS];
	uint32_t tx_su_txbf_bw[ATH12K_HTT_TXBF_NUM_BW_CNTRS];
	uint32_t tx_su_ibf_bw[ATH12K_HTT_TXBF_NUM_BW_CNTRS];
	uint32_t tx_su_ol_bw[ATH12K_HTT_TXBF_NUM_BW_CNTRS];
	uint32_t tx_legacy_ofdm_rate[ATH12K_HTT_TX_PDEV_STATS_NUM_LEGACY_OFDM_STATS];
	uint32_t txbf[ATH12K_HTT_TXBF_NUM_REDUCED_CHAN_TYPES][ATH12K_HTT_TXBF_NUM_BW_CNTRS];
	uint32_t ibf[ATH12K_HTT_TXBF_NUM_REDUCED_CHAN_TYPES][ATH12K_HTT_TXBF_NUM_BW_CNTRS];
	uint32_t ol[ATH12K_HTT_TXBF_NUM_REDUCED_CHAN_TYPES][ATH12K_HTT_TXBF_NUM_BW_CNTRS];
	uint32_t txbf_flag_set_mu_mode;
	uint32_t txbf_flag_set_final_status;
	uint32_t txbf_flag_not_set_verified_txbf_mode;
	uint32_t txbf_flag_not_set_disable_p2p_access;
	uint32_t txbf_flag_not_set_max_nss_in_he160;
	uint32_t txbf_flag_not_set_disable_uldlofdma;
	uint32_t txbf_flag_not_set_mcs_threshold_val;
	uint32_t txbf_flag_not_set_final_status;
} __packed;

struct ath12k_htt_txbf_ofdma_ax_ndpa_stats_elem_t {
	uint32_t ax_ofdma_ndpa_queued;
	uint32_t ax_ofdma_ndpa_tried;
	uint32_t ax_ofdma_ndpa_flush;
	uint32_t ax_ofdma_ndpa_err;
} __packed;

struct ath12k_htt_txbf_ofdma_ax_ndpa_stats_tlv {
	uint32_t num_elems_ax_ndpa_arr;
	uint32_t arr_elem_size_ax_ndpa;
	DECLARE_FLEX_ARRAY(struct ath12k_htt_txbf_ofdma_ax_ndpa_stats_elem_t, ax_ndpa);
} __packed;

struct ath12k_htt_txbf_ofdma_ax_ndp_stats_elem_t {
	uint32_t ax_ofdma_ndp_queued;
	uint32_t ax_ofdma_ndp_tried;
	uint32_t ax_ofdma_ndp_flush;
	uint32_t ax_ofdma_ndp_err;
} __packed;

struct ath12k_htt_txbf_ofdma_ax_ndp_stats_tlv {
	uint32_t num_elems_ax_ndp_arr;
	uint32_t arr_elem_size_ax_ndp;
	DECLARE_FLEX_ARRAY(struct ath12k_htt_txbf_ofdma_ax_ndp_stats_elem_t, ax_ndp);
} __packed;

struct ath12k_htt_txbf_ofdma_ax_brp_stats_elem_t {
	uint32_t ax_ofdma_brp_queued;
	uint32_t ax_ofdma_brp_tried;
	uint32_t ax_ofdma_brp_flushed;
	uint32_t ax_ofdma_brp_err;
	uint32_t ax_ofdma_num_cbf_rcvd;
} __packed;

struct ath12k_htt_txbf_ofdma_ax_brp_stats_tlv {
	uint32_t num_elems_ax_brp_arr;
	uint32_t arr_elem_size_ax_brp;
	DECLARE_FLEX_ARRAY(struct ath12k_htt_txbf_ofdma_ax_brp_stats_elem_t, ax_brp);
} __packed;

struct ath12k_htt_txbf_ofdma_ax_steer_stats_elem_t {
	uint32_t num_ppdu_steer;
	uint32_t num_ppdu_ol;
	uint32_t num_usr_prefetch;
	uint32_t num_usr_sound;
	uint32_t num_usr_force_sound;
} __packed;

struct ath12k_htt_txbf_ofdma_ax_steer_stats_tlv {
	uint32_t num_elems_ax_steer_arr;
	uint32_t arr_elem_size_ax_steer;
	DECLARE_FLEX_ARRAY(struct ath12k_htt_txbf_ofdma_ax_steer_stats_elem_t, ax_steer);
} __packed;

struct ath12k_htt_txbf_ofdma_ax_steer_mpdu_stats_tlv {
	uint32_t ax_ofdma_rbo_steer_mpdus_tried;
	uint32_t ax_ofdma_rbo_steer_mpdus_failed;
	uint32_t ax_ofdma_sifs_steer_mpdus_tried;
	uint32_t ax_ofdma_sifs_steer_mpdus_failed;
} __packed;

enum ath12k_htt_stats_page_lock_state {
	ATH12K_HTT_STATS_PAGE_LOCKED	= 0,
	ATH12K_HTT_STATS_PAGE_UNLOCKED	= 1,
	ATH12K_NUM_PG_LOCK_STATE
};

#define ATH12K_PAGER_MAX	10

#define ATH12K_HTT_DLPAGER_ASYNC_LOCK_PG_CNT_INFO0	GENMASK(7, 0)
#define ATH12K_HTT_DLPAGER_SYNC_LOCK_PG_CNT_INFO0	GENMASK(15, 8)
#define ATH12K_HTT_DLPAGER_TOTAL_LOCK_PAGES_INFO1	GENMASK(15, 0)
#define ATH12K_HTT_DLPAGER_TOTAL_FREE_PAGES_INFO1	GENMASK(31, 16)
#define ATH12K_HTT_DLPAGER_TOTAL_LOCK_PAGES_INFO2	GENMASK(15, 0)
#define ATH12K_HTT_DLPAGER_TOTAL_FREE_PAGES_INFO2	GENMASK(31, 16)

struct ath12k_htt_pgs_info {
	uint32_t page_num;
	uint32_t num_pgs;
	uint32_t ts_lsb;
	uint32_t ts_msb;
} __packed;

struct ath12k_htt_dl_pager_stats_tlv {
	uint32_t info0;
	uint32_t info1;
	uint32_t info2;
	struct ath12k_htt_pgs_info pgs_info[ATH12K_NUM_PG_LOCK_STATE][ATH12K_PAGER_MAX];
} __packed;

#define ATH12K_HTT_STATS_MAX_CHAINS		8
#define ATH12K_HTT_MAX_RX_PKT_CNT		8
#define ATH12K_HTT_MAX_RX_PKT_CRC_PASS_CNT	8
#define ATH12K_HTT_MAX_PER_BLK_ERR_CNT		20
#define ATH12K_HTT_MAX_RX_OTA_ERR_CNT		14
#define ATH12K_HTT_MAX_CH_PWR_INFO_SIZE		16

struct ath12k_htt_phy_stats_tlv {
	int32_t nf_chain[ATH12K_HTT_STATS_MAX_CHAINS];
	uint32_t false_radar_cnt;
	uint32_t radar_cs_cnt;
	int32_t ani_level;
	uint32_t fw_run_time;
	int32_t runtime_nf_chain[ATH12K_HTT_STATS_MAX_CHAINS];
} __packed;

struct ath12k_htt_phy_counters_tlv {
	uint32_t rx_ofdma_timing_err_cnt;
	uint32_t rx_cck_fail_cnt;
	uint32_t mactx_abort_cnt;
	uint32_t macrx_abort_cnt;
	uint32_t phytx_abort_cnt;
	uint32_t phyrx_abort_cnt;
	uint32_t phyrx_defer_abort_cnt;
	uint32_t rx_gain_adj_lstf_event_cnt;
	uint32_t rx_gain_adj_non_legacy_cnt;
	uint32_t rx_pkt_cnt[ATH12K_HTT_MAX_RX_PKT_CNT];
	uint32_t rx_pkt_crc_pass_cnt[ATH12K_HTT_MAX_RX_PKT_CRC_PASS_CNT];
	uint32_t per_blk_err_cnt[ATH12K_HTT_MAX_PER_BLK_ERR_CNT];
	uint32_t rx_ota_err_cnt[ATH12K_HTT_MAX_RX_OTA_ERR_CNT];
} __packed;

struct ath12k_htt_phy_reset_stats_tlv {
	uint32_t pdev_id;
	uint32_t chan_mhz;
	uint32_t chan_band_center_freq1;
	uint32_t chan_band_center_freq2;
	uint32_t chan_phy_mode;
	uint32_t chan_flags;
	uint32_t chan_num;
	uint32_t reset_cause;
	uint32_t prev_reset_cause;
	uint32_t phy_warm_reset_src;
	uint32_t rx_gain_tbl_mode;
	uint32_t xbar_val;
	uint32_t force_calibration;
	uint32_t phyrf_mode;
	uint32_t phy_homechan;
	uint32_t phy_tx_ch_mask;
	uint32_t phy_rx_ch_mask;
	uint32_t phybb_ini_mask;
	uint32_t phyrf_ini_mask;
	uint32_t phy_dfs_en_mask;
	uint32_t phy_sscan_en_mask;
	uint32_t phy_synth_sel_mask;
	uint32_t phy_adfs_freq;
	uint32_t cck_fir_settings;
	uint32_t phy_dyn_pri_chan;
	uint32_t cca_thresh;
	uint32_t dyn_cca_status;
	uint32_t rxdesense_thresh_hw;
	uint32_t rxdesense_thresh_sw;
} __packed;

struct ath12k_htt_phy_reset_counters_tlv {
	uint32_t pdev_id;
	uint32_t cf_active_low_fail_cnt;
	uint32_t cf_active_low_pass_cnt;
	uint32_t phy_off_through_vreg_cnt;
	uint32_t force_calibration_cnt;
	uint32_t rf_mode_switch_phy_off_cnt;
	uint32_t temperature_recal_cnt;
} __packed;

struct ath12k_htt_phy_tpc_stats_tlv {
	uint32_t pdev_id;
	uint32_t tx_power_scale;
	uint32_t tx_power_scale_db;
	uint32_t min_negative_tx_power;
	uint32_t reg_ctl_domain;
	uint32_t max_reg_allowed_power[ATH12K_HTT_STATS_MAX_CHAINS];
	uint32_t max_reg_allowed_power_6ghz[ATH12K_HTT_STATS_MAX_CHAINS];
	uint32_t twice_max_rd_power;
	uint32_t max_tx_power;
	uint32_t home_max_tx_power;
	uint32_t psd_power;
	uint32_t eirp_power;
	uint32_t power_type_6ghz;
	uint32_t sub_band_cfreq[ATH12K_HTT_MAX_CH_PWR_INFO_SIZE];
	uint32_t sub_band_txpower[ATH12K_HTT_MAX_CH_PWR_INFO_SIZE];
} __packed;

struct ath12k_htt_t2h_soc_txrx_stats_common_tlv {
	uint32_t inv_peers_msdu_drop_count_hi;
	uint32_t inv_peers_msdu_drop_count_lo;
} __packed;

#define ATH12K_HTT_AST_PDEV_ID_INFO		GENMASK(1, 0)
#define ATH12K_HTT_AST_VDEV_ID_INFO		GENMASK(9, 2)
#define ATH12K_HTT_AST_NEXT_HOP_INFO		BIT(10)
#define ATH12K_HTT_AST_MCAST_INFO		BIT(11)
#define ATH12K_HTT_AST_MONITOR_DIRECT_INFO	BIT(12)
#define ATH12K_HTT_AST_MESH_STA_INFO		BIT(13)
#define ATH12K_HTT_AST_MEC_INFO			BIT(14)
#define ATH12K_HTT_AST_INTRA_BSS_INFO		BIT(15)

struct ath12k_htt_ast_entry_tlv {
	uint32_t sw_peer_id;
	uint32_t ast_index;
    uint8_t mac_addr[ETH_ALEN];
	uint32_t info;
} __packed;

enum ath12k_htt_stats_direction {
	ATH12K_HTT_STATS_DIRECTION_TX,
	ATH12K_HTT_STATS_DIRECTION_RX
};

enum ath12k_htt_stats_ppdu_type {
	ATH12K_HTT_STATS_PPDU_TYPE_MODE_SU,
	ATH12K_HTT_STATS_PPDU_TYPE_DL_MU_MIMO,
	ATH12K_HTT_STATS_PPDU_TYPE_UL_MU_MIMO,
	ATH12K_HTT_STATS_PPDU_TYPE_DL_MU_OFDMA,
	ATH12K_HTT_STATS_PPDU_TYPE_UL_MU_OFDMA
};

enum ath12k_htt_stats_param_type {
	ATH12K_HTT_STATS_PREAM_OFDM,
	ATH12K_HTT_STATS_PREAM_CCK,
	ATH12K_HTT_STATS_PREAM_HT,
	ATH12K_HTT_STATS_PREAM_VHT,
	ATH12K_HTT_STATS_PREAM_HE,
	ATH12K_HTT_STATS_PREAM_EHT,
	ATH12K_HTT_STATS_PREAM_RSVD1,
	ATH12K_HTT_STATS_PREAM_COUNT,
};

#define ATH12K_HTT_PUNCT_STATS_MAX_SUBBAND_CNT	32

struct ath12k_htt_pdev_puncture_stats_tlv {
	uint32_t mac_id__word;
	uint32_t direction;
	uint32_t preamble;
	uint32_t ppdu_type;
	uint32_t subband_cnt;
	uint32_t last_used_pattern_mask;
	uint32_t num_subbands_used_cnt[ATH12K_HTT_PUNCT_STATS_MAX_SUBBAND_CNT];
} __packed;

struct ath12k_htt_dmac_reset_stats_tlv {
	uint32_t reset_count;
	uint32_t reset_time_lo_ms;
	uint32_t reset_time_hi_ms;
	uint32_t disengage_time_lo_ms;
	uint32_t disengage_time_hi_ms;
	uint32_t engage_time_lo_ms;
	uint32_t engage_time_hi_ms;
	uint32_t disengage_count;
	uint32_t engage_count;
	uint32_t drain_dest_ring_mask;
} __packed;

struct ath12k_htt_pdev_sched_algo_ofdma_stats_tlv {
	uint32_t mac_id__word;
	uint32_t rate_based_dlofdma_enabled_cnt[ATH12K_HTT_NUM_AC_WMM];
	uint32_t rate_based_dlofdma_disabled_cnt[ATH12K_HTT_NUM_AC_WMM];
	uint32_t rate_based_dlofdma_probing_cnt[ATH12K_HTT_NUM_AC_WMM];
	uint32_t rate_based_dlofdma_monitor_cnt[ATH12K_HTT_NUM_AC_WMM];
	uint32_t chan_acc_lat_based_dlofdma_enabled_cnt[ATH12K_HTT_NUM_AC_WMM];
	uint32_t chan_acc_lat_based_dlofdma_disabled_cnt[ATH12K_HTT_NUM_AC_WMM];
	uint32_t chan_acc_lat_based_dlofdma_monitor_cnt[ATH12K_HTT_NUM_AC_WMM];
	uint32_t downgrade_to_dl_su_ru_alloc_fail[ATH12K_HTT_NUM_AC_WMM];
	uint32_t candidate_list_single_user_disable_ofdma[ATH12K_HTT_NUM_AC_WMM];
	uint32_t dl_cand_list_dropped_high_ul_qos_weight[ATH12K_HTT_NUM_AC_WMM];
	uint32_t ax_dlofdma_disabled_due_to_pipelining[ATH12K_HTT_NUM_AC_WMM];
	uint32_t dlofdma_disabled_su_only_eligible[ATH12K_HTT_NUM_AC_WMM];
	uint32_t dlofdma_disabled_consec_no_mpdus_tried[ATH12K_HTT_NUM_AC_WMM];
	uint32_t dlofdma_disabled_consec_no_mpdus_success[ATH12K_HTT_NUM_AC_WMM];
} __packed;

#define ATH12K_HTT_TX_PDEV_STATS_NUM_BW_CNTRS		4
#define ATH12K_HTT_PDEV_STAT_NUM_SPATIAL_STREAMS	8
#define ATH12K_HTT_TXBF_RATE_STAT_NUM_MCS_CNTRS		14

enum ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE {
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_26,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_52,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_52_26,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_106,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_106_26,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_242,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_484,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_484_242,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_996,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_996_484,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_996_484_242,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_996x2,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_996x2_484,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_996x3,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_996x3_484,
	ATH12K_HTT_TX_RX_PDEV_STATS_BE_RU_SIZE_996x4,
	ATH12K_HTT_TX_RX_PDEV_NUM_BE_RU_SIZE_CNTRS,
};

enum ATH12K_HTT_RC_MODE {
	ATH12K_HTT_RC_MODE_SU_OL,
	ATH12K_HTT_RC_MODE_SU_BF,
	ATH12K_HTT_RC_MODE_MU1_INTF,
	ATH12K_HTT_RC_MODE_MU2_INTF,
	ATH12K_HTT_RC_MODE_MU3_INTF,
	ATH12K_HTT_RC_MODE_MU4_INTF,
	ATH12K_HTT_RC_MODE_MU5_INTF,
	ATH12K_HTT_RC_MODE_MU6_INTF,
	ATH12K_HTT_RC_MODE_MU7_INTF,
	ATH12K_HTT_RC_MODE_2D_COUNT
};

enum ath12k_htt_stats_rc_mode {
	ATH12K_HTT_STATS_RC_MODE_DLSU     = 0,
	ATH12K_HTT_STATS_RC_MODE_DLMUMIMO = 1,
	ATH12K_HTT_STATS_RC_MODE_DLOFDMA  = 2,
	ATH12K_HTT_STATS_RC_MODE_ULMUMIMO = 3,
	ATH12K_HTT_STATS_RC_MODE_ULOFDMA  = 4,
};

enum ath12k_htt_stats_ru_type {
	ATH12K_HTT_STATS_RU_TYPE_INVALID,
	ATH12K_HTT_STATS_RU_TYPE_SINGLE_RU_ONLY,
	ATH12K_HTT_STATS_RU_TYPE_SINGLE_AND_MULTI_RU,
};

struct ath12k_htt_tx_rate_stats {
	uint32_t ppdus_tried;
	uint32_t ppdus_ack_failed;
	uint32_t mpdus_tried;
	uint32_t mpdus_failed;
} __packed;

struct ath12k_htt_tx_per_rate_stats_tlv {
	uint32_t rc_mode;
	uint32_t last_probed_mcs;
	uint32_t last_probed_nss;
	uint32_t last_probed_bw;
	struct ath12k_htt_tx_rate_stats per_bw[ATH12K_HTT_TX_PDEV_STATS_NUM_BW_CNTRS];
	struct ath12k_htt_tx_rate_stats per_nss[ATH12K_HTT_PDEV_STAT_NUM_SPATIAL_STREAMS];
	struct ath12k_htt_tx_rate_stats per_mcs[ATH12K_HTT_TXBF_RATE_STAT_NUM_MCS_CNTRS];
	struct ath12k_htt_tx_rate_stats per_bw320;
	uint32_t probe_cnt[ATH12K_HTT_RC_MODE_2D_COUNT];
	uint32_t ru_type;
	struct ath12k_htt_tx_rate_stats ru[ATH12K_HTT_TX_RX_PDEV_NUM_BE_RU_SIZE_CNTRS];
} __packed;

#define ATH12K_HTT_TX_PDEV_NUM_BE_MCS_CNTRS		16
#define ATH12K_HTT_TX_PDEV_NUM_BE_BW_CNTRS		5
#define ATH12K_HTT_TX_PDEV_NUM_EHT_SIG_MCS_CNTRS	4
#define ATH12K_HTT_TX_PDEV_NUM_GI_CNTRS			4

struct ath12k_htt_tx_pdev_rate_stats_be_ofdma_tlv {
	uint32_t mac_id__word;
	uint32_t be_ofdma_tx_ldpc;
	uint32_t be_ofdma_tx_mcs[ATH12K_HTT_TX_PDEV_NUM_BE_MCS_CNTRS];
	uint32_t be_ofdma_tx_nss[ATH12K_HTT_PDEV_STAT_NUM_SPATIAL_STREAMS];
	uint32_t be_ofdma_tx_bw[ATH12K_HTT_TX_PDEV_NUM_BE_BW_CNTRS];
	uint32_t gi[ATH12K_HTT_TX_PDEV_NUM_GI_CNTRS][ATH12K_HTT_TX_PDEV_NUM_BE_MCS_CNTRS];
	uint32_t be_ofdma_tx_ru_size[ATH12K_HTT_TX_RX_PDEV_NUM_BE_RU_SIZE_CNTRS];
	uint32_t be_ofdma_eht_sig_mcs[ATH12K_HTT_TX_PDEV_NUM_EHT_SIG_MCS_CNTRS];
} __packed;

struct ath12k_htt_pdev_mbssid_ctrl_frame_tlv {
	uint32_t mac_id__word;
	uint32_t basic_trigger_across_bss;
	uint32_t basic_trigger_within_bss;
	uint32_t bsr_trigger_across_bss;
	uint32_t bsr_trigger_within_bss;
	uint32_t mu_rts_across_bss;
	uint32_t mu_rts_within_bss;
	uint32_t ul_mumimo_trigger_across_bss;
	uint32_t ul_mumimo_trigger_within_bss;
} __packed;

#endif
