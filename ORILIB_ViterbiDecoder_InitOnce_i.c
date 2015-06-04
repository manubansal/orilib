/**
Atomix project, ORILIB_ViterbiDecoder_InitOnce_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

void ORILIB_ViterbiDecoder_InitOnce_i (
		OUT ORILIB_t_ViterbiDecoderState * const state,
		CF  ORILIB_t_ViterbiDecoderInitOnceConf * conf
		){
	ORILIB_ViterDecoder_t_VcpInstance which_vcp = conf->which_vcp;
	Vitdec_Profile * vdp = &(state->profile);
	state->which_vcp = which_vcp;

	switch (conf->mode) {
	    case VITDEC_HEADCONVGT:
	      //-------------------------------- Generate configuration for head decode ---------------------------------------
	      vitdec_profile_wifiGee_populate_convergent(vdp, conf->convergenceLength);
	      //---------------------------------------------------------------------------------------------------------------
	      break;

	    case VITDEC_MIDCONVGT:
	      //-------------------------------- Generate configuration for mid decode ---------------------------------------
	      vitdec_profile_wifiGee_populate_convergent(vdp, conf->convergenceLength);
	      //---------------------------------------------------------------------------------------------------------------
	      break;

	    
	    case VITDEC_TAILCONVGT:
	      //-------------------------------- Generate configuration for tail decode ---------------------------------------
	      vitdec_profile_wifiGee_populate_tailed(vdp, conf->convergenceLength);
	      //---------------------------------------------------------------------------------------------------------------
	      break;


	    case VITDEC_HEADTAIL:
	      //-------------------------------- Generate configuration for single head tail decode ---------------------------
	      vitdec_profile_wifiGee_populate_tailed(vdp, conf->convergenceLength);
	      //---------------------------------------------------------------------------------------------------------------
	      break;
	}
	

	DEBUG(
	LOG_PRINTF("ORILIB_ViterbiDecoder_InitOnce: mode=%d, which_vcp=%d, convergenceLength=%d\n", conf->mode, conf->which_vcp, conf->convergenceLength);
	)
}

void ORILIB_ViterbiDecoder_InitOnce_i_conf (
		CF  ORILIB_t_ViterbiDecoderInitOnceConf * conf,
		IN  const ORILIB_ViterDecoder_t_VcpInstance which_vcp,
		IN  const Vitdec_Mode mode,
		IN  const Vitdec_ConvergenceLength convergenceLength
	){
	conf->which_vcp = which_vcp;
	conf->mode = mode;
	conf->convergenceLength = convergenceLength;
}
