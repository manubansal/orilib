/**
Atomix project, ORILIB_ViterbiDecoderVCP_algorithms.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_VITERBIDECODERVCP_ALGORITHMS_H_
#define ORILIB_VITERBIDECODERVCP_ALGORITHMS_H_


#include <osl/inc/or_types.h>
#include <osl/vitdec/inc/vcpdriver/vcpdriver_api.h>
#include <osl/vitdec/inc/vcpdriver/vcpdriver.h>
#include "ORILIB_ViterbiDecoder_t.h"

void ORILIB_ViterbiDecoderVCP_SingleHeadTailDecode(
	IN BitsSoft const * const restrict branch_metrics,
	OUT BitsPacked_ll * const restrict hard_decisions,
	ORILIB_t_ViterbiDecoderState * const state
	);

void ORILIB_ViterbiDecoder_CacheBranchMetrics_algo (
	IN  BitsSoft const * const branch_metrics,
	IN  ORILIB_t_ViterbiDecoderState * const inpState,
	OUT ORILIB_t_ViterbiDecoderState * const outState,
	IN  Uint32 num_branch_metrics_per_ofdm_symbol
	);


static inline
void ORILIB_ViterbiDecoderVCP_algorithm (
	IN BitsSoft const * const restrict branch_metrics,
	OUT BitsPacked_ll * const restrict hard_decisions,
	ORILIB_t_ViterbiDecoderState * const state
	){

	Vitdec_Profile * vdp = &(state->profile);
	VCP2_ConfigIc * config = &(state->config);
	uint64_t timestamps[20];
	
	vcp2_decode(
		state->which_vcp, 
		config, 
		(uint32_t*)branch_metrics, 
		(uint32_t*)hard_decisions, 
		vdp->inputBM_frames, 
		vdp->outputHD_bytes_rounded, 
		&(state->cword), 
		timestamps);


}

static inline
void ORILIB_ViterbiDecoderVCP_Dynamic_algorithm (
	IN BitsSoft const * const restrict branch_metrics,
	OUT BitsPacked_ll * const restrict hard_decisions,
	ORILIB_t_ViterbiDecoderState * const state,
	ORILIB_t_ViterbiDecoderWFCState	* const wfcState
	){

	Vitdec_Profile * vdp = &(state->profile);
	VCP2_ConfigIc * config = &(state->config);
	uint64_t timestamps[20];
	
	vcp2_decode(
		wfcState->which_vcp, 
		config, 
		(uint32_t*)branch_metrics, 
		(uint32_t*)hard_decisions, 
		vdp->inputBM_frames, 
		vdp->outputHD_bytes_rounded, 
		&(state->cword), 
		timestamps);


}

//	IN  void				* inpBufBranchMetrics,
//	OUT void				* outBufDecisions

static inline
void ORILIB_ViterbiDecoderVCP_WaitForCompletion (
	IN  ORILIB_t_ViterbiDecoderWFCState 	* const wfcState
	){

	//uint64_t dummy[20];
	//Vitdec_Profile * vdp = &(state->profile);

	//vcp2_waitForCompletion(
	//	vdp->numSymProcExpected,
	//	state->which_vcp,
	//	dummy,
	//	BREAKTYPE_SYM,
	//	dummy);
	vcp2_waitForCompletion(
		wfcState->numSymProcExpected,
		wfcState->which_vcp,
		//dummy,
		BREAKTYPE_SYM
		//BREAKTYPE_INT
		//dummy
		);

/*	vcp2_postDecodeStat(
			state->which_vcp, 
			 (uint32_t*)(state->internal_output_cache), 
			16); */
	/////vcp2_waitForCompletion(state->last_num_bits_requested,state->which_vcp,dummy,BREAKTYPE_SYM,dummy);

//	DEBUG(
//			static int count = 0;
//			printf("decoder count: %3d    \n", count++);
////			TEST_printDebug_decodedBits((Uint8 *)output_cache_ptr_32, state->output_frame_length/8);
//			TEST_printDebug_decodedBits((Uint8 *)&state->internal_output_cache[(state->output_cache_pingpong_write_idx)], state->output_frame_length/8 + 5);
//	)




}

#endif

