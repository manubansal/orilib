/**
Atomix project, ORILIB_ViterbiDecoder_InitOnce_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_VITERBIDECODER_INITONCE_I_H_

#include "ORILIB_ViterbiDecoder_InitOnce_t.h"

void ORILIB_ViterbiDecoder_InitOnce_i (
		OUT ORILIB_t_ViterbiDecoderState * const state,
		CF  ORILIB_t_ViterbiDecoderInitOnceConf * conf
		);

void ORILIB_ViterbiDecoder_InitOnce_i_conf (
		CF  ORILIB_t_ViterbiDecoderInitOnceConf * conf,
		IN  const ORILIB_ViterDecoder_t_VcpInstance which_vcp,
		IN  const Vitdec_Mode mode,
		IN  const Vitdec_ConvergenceLength convergenceLength
		);

#endif

