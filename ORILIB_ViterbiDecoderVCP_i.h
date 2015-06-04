/**
Atomix project, ORILIB_ViterbiDecoderVCP_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_VITERBIDECODERVCP_I_H_
#define ORILIB_VITERBIDECODERVCP_I_H_


void ORILIB_ViterbiDecoderVCP_i (
	IN  void				* inpBufBranchMetrics,
	IN  ORILIB_t_ViterbiDecoderState	* inpBufState,
	OUT void				* outBufDecisions,
	OUT ORILIB_t_ViterbiDecoderWFCState	* outBufWfcState
	);

#endif

