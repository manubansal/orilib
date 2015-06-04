/**
Atomix project, ORILIB_ViterbiDecoderVCP_Dynamic_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_VITERBIDECODERVCP_DYNAMIC_I_H 

#include "ORILIB_ViterbiDecoderVCP_Dynamic_t.h"

void ORILIB_ViterbiDecoderVCP_Dynamic_i (
	IN  void				* inpBufBranchMetrics,
	IN  ORILIB_t_ViterbiDecoderState	* inpBufState,
	IN  ORILIB_t_ViterbiDecoderWFCState	* inoutBufWfcState,
	OUT void				* outBufDecisions
	);

#endif /* ORILIB_VITERBIDECODERVCP_DYNAMIC_I_H */
