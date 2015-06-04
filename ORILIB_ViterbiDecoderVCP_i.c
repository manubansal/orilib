/**
Atomix project, ORILIB_ViterbiDecoderVCP_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_ViterbiDecoderVCP_algorithms.h"


void ORILIB_ViterbiDecoderVCP_i (
	IN  void				* inpBufBranchMetrics,
	IN  ORILIB_t_ViterbiDecoderState	* inpBufState,
	OUT void				* outBufDecisions,
	OUT ORILIB_t_ViterbiDecoderWFCState	* outBufWfcState
	){

	ORILIB_ViterbiDecoderVCP_algorithm (
	    (BitsSoft *)inpBufBranchMetrics,
	    outBufDecisions,	
	    inpBufState	
	    );

	outBufWfcState->which_vcp 		= inpBufState->which_vcp;
	outBufWfcState->numSymProcExpected 	= inpBufState->profile.numSymProcExpected;
}
