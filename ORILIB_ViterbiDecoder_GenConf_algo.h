/**
Atomix project, ORILIB_ViterbiDecoder_GenConf_algo.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_VITERBIDECODER_GENCONF_ALGO_H_

#include "ORILIB_ViterbiDecoder_t.h"
#include <osl/inc/amem4cpy_inl.h>


//state is in-out
static inline void ORILIB_ViterbiDecoder_GenConfip_internal (
	IN  Uint32 num_bits_to_decode,
	IN  Vitdec_Mode mode,
	OUT ORILIB_t_ViterbiDecoderState * const state
	){

	state->num_bits_to_decode = num_bits_to_decode;
	
	Vitdec_Profile * vdp;
	vdp = &(state->profile);

	switch (mode) {
	    case VITDEC_HEADCONVGT:
	      //-------------------------------- Generate configuration for head decode ---------------------------------------
	      vitdec_profile_setDataLength_head_convergent(state->num_bits_to_decode, vdp);
	      //---------------------------------------------------------------------------------------------------------------
	      break;

	    case VITDEC_MIDCONVGT:
	      //-------------------------------- Generate configuration for mid decode ---------------------------------------
	      vitdec_profile_setDataLength_mid_convergent(state->num_bits_to_decode, vdp);
	      //---------------------------------------------------------------------------------------------------------------
	      break;

	    
	    case VITDEC_TAILCONVGT:
	      //-------------------------------- Generate configuration for tail decode ---------------------------------------
	      vitdec_profile_setDataLength_tail_nohead(state->num_bits_to_decode, vdp);
	      //---------------------------------------------------------------------------------------------------------------
	      break;


	    case VITDEC_HEADTAIL:
	      //-------------------------------- Generate configuration for single head tail decode ---------------------------
	      vitdec_profile_setDataLength_head_tail(state->num_bits_to_decode, vdp);
	      //---------------------------------------------------------------------------------------------------------------
	      break;
	}


	//---------------------------------------------------------------------------------------------------------------
	vcp2_genConf(vdp, &(state->config));
	//---------------------------------------------------------------------------------------------------------------

	DEBUG(
	printf("ORILIB_ViterbiDecoder_InitPerPacket_internal: num_bits_to_decode=%d\n", num_bits_to_decode); 
	)
}

static inline void ORILIB_ViterbiDecoder_GenConf_internal (
	IN  Uint32 num_bits_to_decode,
	IN  Vitdec_Mode mode,
	//IN  Vitdec_ConvergenceLength convergenceLength,
	IN  ORILIB_t_ViterbiDecoderState * const inpState,
	OUT ORILIB_t_ViterbiDecoderState * const outState
	){

	_amem4cpy(outState, inpState, sizeof(ORILIB_t_ViterbiDecoderState)/4);
	ORILIB_t_ViterbiDecoderState * state = outState;

	ORILIB_ViterbiDecoder_GenConfip_internal (num_bits_to_decode, mode, state);
}

#endif

