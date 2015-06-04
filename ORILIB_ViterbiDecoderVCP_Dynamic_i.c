/**
Atomix project, ORILIB_ViterbiDecoderVCP_Dynamic_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_ViterbiDecoderVCP_algorithms.h"


void ORILIB_ViterbiDecoderVCP_Dynamic_i (
	IN  void				* inpBufBranchMetrics,
	IN  ORILIB_t_ViterbiDecoderState	* inpBufState,
	IN  ORILIB_t_ViterbiDecoderWFCState	* inoutBufWfcState,
	OUT void				* outBufDecisions
	){

	ORILIB_ViterbiDecoderVCP_Dynamic_algorithm (
	    (BitsSoft *)inpBufBranchMetrics,
	    outBufDecisions,	
	    inpBufState,
	    inoutBufWfcState
	    );

	inoutBufWfcState->numSymProcExpected 	= inpBufState->profile.numSymProcExpected;
}
