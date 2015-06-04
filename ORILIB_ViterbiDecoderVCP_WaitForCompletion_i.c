/**
Atomix project, ORILIB_ViterbiDecoderVCP_WaitForCompletion_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_ViterbiDecoderVCP_WaitForCompletion_t.h"
#include "ORILIB_ViterbiDecoderVCP_algorithms.h"

//	IN  void				* inpBufBranchMetrics,
//	OUT void				* outBufDecisions

//wfcState is treated as an in-out buffer (wired with + or * operator), 
//because we modify the wfc state in this function
void ORILIB_ViterbiDecoderVCP_WaitForCompletion_i (
	IN  ORILIB_t_ViterbiDecoderWFCState 	* const wfcState
	){
	
	//ORILIB_ViterbiDecoderVCP_WaitForCompletion(inpBufBranchMetrics, wfcState, outBufDecisions);
	ORILIB_ViterbiDecoderVCP_WaitForCompletion(wfcState);

	wfcState->numSymProcExpected = 0;	//this is the always-true WFC state
}
