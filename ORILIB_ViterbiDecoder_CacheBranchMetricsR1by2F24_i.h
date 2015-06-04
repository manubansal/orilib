/**
Atomix project, ORILIB_ViterbiDecoder_CacheBranchMetricsR1by2F24_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_VITERBIDECODER_CACHEBRANCHMETRICSR1BY2F24_I_H_
#define ORILIB_VITERBIDECODER_CACHEBRANCHMETRICSR1BY2F24_I_H_

#include "ORILIB_ViterbiDecoderVCP_algorithms.h"
#include "ORILIB_ViterbiDecoder_t.h"

void ORILIB_ViterbiDecoder_CacheBranchMetricsR1by2F24_i (
	IN  ORILIB_t_SoftBitBuf48 * inpBranchMetrics,
	IN  ORILIB_t_ViterbiDecoderState * const inpState,
	OUT ORILIB_t_ViterbiDecoderState * const outState
	);

#endif

