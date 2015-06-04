/**
Atomix project, ORILIB_ViterbiDecoderVCP_InitIdx_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_VITERBIDECODERVCP_INITIDX_I_H
#define ORILIB_VITERBIDECODERVCP_INITIDX_I_H 

#include "ORILIB_ViterbiDecoderVCP_InitIdx_t.h"


void ORILIB_ViterbiDecoderVCP_InitIdx_i (
		OUT ORILIB_t_ViterbiDecoderIdx * const idx,
		CF  ORILIB_t_ViterbiDecoderVCP_InitIdxConf * conf
		);

void ORILIB_ViterbiDecoderVCP_InitIdx_i_conf (
		CF  ORILIB_t_ViterbiDecoderVCP_InitIdxConf * conf,
		IN  const ORILIB_ViterDecoder_t_VcpInstance which_vcp
		);


#endif /* ORILIB_VITERBIDECODERVCP_INITIDX_I_H */
