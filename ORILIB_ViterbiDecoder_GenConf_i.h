/**
Atomix project, ORILIB_ViterbiDecoder_GenConf_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_VITERTIDECODER_GENCONF_I_H_

#include "ORILIB_ViterbiDecoder_GenConf_t.h"

void ORILIB_ViterbiDecoder_GenConf_i (
	IN  Vitdec_ViterbiDecodingMap const * const restrict vitdecMap,
	IN  ORILIB_t_ViterbiDecoderState * const inpState,
	OUT ORILIB_t_ViterbiDecoderState * const outState,
	CF  ORILIB_t_ViterbiDecoderGenConfConf * const conf
	);


void ORILIB_ViterbiDecoder_GenConf_i_conf (
	CF  ORILIB_t_ViterbiDecoderGenConfConf * const conf,
	IN  Vitdec_Mode mode
	);

#endif

