/**
Atomix project, ORILIB_ViterbiDecoderVCP_InitIdx_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_ViterbiDecoderVCP_InitIdx_t.h"

void ORILIB_ViterbiDecoderVCP_InitIdx_i (
		OUT ORILIB_t_ViterbiDecoderIdx * const idx,
		CF  ORILIB_t_ViterbiDecoderVCP_InitIdxConf * conf
		){
	ORILIB_ViterDecoder_t_VcpInstance which_vcp = conf->which_vcp;
	idx->which_vcp = which_vcp;
}

void ORILIB_ViterbiDecoderVCP_InitIdx_i_conf (
		CF  ORILIB_t_ViterbiDecoderVCP_InitIdxConf * conf,
		IN  const ORILIB_ViterDecoder_t_VcpInstance which_vcp
	){
	conf->which_vcp = which_vcp;
}
