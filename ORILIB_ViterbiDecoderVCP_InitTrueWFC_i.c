/**
Atomix project, ORILIB_ViterbiDecoderVCP_InitTrueWFC_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/



#include "ORILIB_ViterbiDecoderVCP_WaitForCompletion_t.h"
#include "ORILIB_ViterbiDecoderVCP_algorithms.h"

void ORILIB_ViterbiDecoderVCP_InitTrueWFC_i (
	OUT ORILIB_t_ViterbiDecoderWFCState * wfcState,
	CF  ORILIB_t_ViterbiDecoderWFCConf * conf
	){
	
	wfcState->which_vcp = conf->which_vcp;
	wfcState->numSymProcExpected = 0;	//this is the always-true WFC state
}

void ORILIB_ViterbiDecoderVCP_InitTrueWFC_i_conf (
	CF  ORILIB_t_ViterbiDecoderWFCConf * conf,
	IN  const ORILIB_ViterDecoder_t_VcpInstance which_vcp
	){
	conf->which_vcp = which_vcp;
}
