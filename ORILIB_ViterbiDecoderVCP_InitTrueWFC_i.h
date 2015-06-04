/**
Atomix project, ORILIB_ViterbiDecoderVCP_InitTrueWFC_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_VITERBIDECODERVCP_INITTRUEWFC_I_H
#define ORILIB_VITERBIDECODERVCP_INITTRUEWFC_I_H 

#include "ORILIB_ViterbiDecoderVCP_WaitForCompletion_t.h"

void ORILIB_ViterbiDecoderVCP_InitTrueWFC_i (
	OUT ORILIB_t_ViterbiDecoderWFCState * wfcState,
	CF  ORILIB_t_ViterbiDecoderWFCConf * conf
	);

void ORILIB_ViterbiDecoderVCP_InitTrueWFC_i_conf (
	CF  ORILIB_t_ViterbiDecoderWFCConf * conf,
	IN  const ORILIB_ViterDecoder_t_VcpInstance which_vcp
	);

#endif /* ORILIB_VITERBIDECODERVCP_INITTRUEWFC_I_H */
