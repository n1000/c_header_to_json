#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "util.h"
#include "test2_input.h"
#include "out2.c"

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    struct ath12k_htt_tx_pdev_stats_cmn_tlv a = {};
    struct ath12k_htt_tx_pdev_mu_ppdu_dist_stats_tlv b = {};
    struct ath12k_htt_tx_pdev_rate_stats_be_ofdma_tlv c = {};
    i_printf(0, "{\n");
    dump_json_struct_ath12k_htt_tx_pdev_stats_cmn_tlv(1, &a);
    i_printf(0, ",\n");
    dump_json_struct_ath12k_htt_tx_pdev_mu_ppdu_dist_stats_tlv(1, &b);
    i_printf(0, ",\n");
    dump_json_struct_ath12k_htt_tx_pdev_rate_stats_be_ofdma_tlv(1, &c);
    i_printf(0, "\n}\n");
}
