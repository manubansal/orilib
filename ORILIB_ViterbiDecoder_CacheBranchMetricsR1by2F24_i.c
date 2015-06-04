/**
Atomix project, ORILIB_ViterbiDecoder_CacheBranchMetricsR1by2F24_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_ViterbiDecoderVCP_algorithms.h"
#include "ORILIB_ViterbiDecoder_t.h"

//void ViterbiDecoderWifi_CacheInputMetrics(
//	IN BitsSoft const * const branch_metrics,
//	ViterbiDecoderWifiState * const state
//	){
void ORILIB_ViterbiDecoder_CacheBranchMetricsR1by2F24_i (
	IN  ORILIB_t_SoftBitBuf48 * inpBranchMetrics,
	IN  ORILIB_t_ViterbiDecoderState * const inpState,
	OUT ORILIB_t_ViterbiDecoderState * const outState
	){
	//uint32_t num_branch_metrics_per_ofdm_symbol = ((kWifiGeeDataRatesInfo[state->data_rate].num_data_bits_per_ofdm_symbol)<<1);
	Uint32 num_branch_metrics_per_ofdm_symbol = 48;
	BitsSoft * branch_metrics = inpBranchMetrics->softBits;

	ORILIB_ViterbiDecoder_CacheBranchMetrics_algo(
		branch_metrics, 
		inpState, 
		outState, 
		num_branch_metrics_per_ofdm_symbol
		);
}
