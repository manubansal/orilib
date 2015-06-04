/**
Atomix project, ORILIB_TailZeroer24_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_TailZeroer24_t.h"

void ORILIB_TailZeroer24_i (
	IN  ORILIB_t_BitBucket24         * inpBuf,
	IN  ORILIB_TailZeroer24_t_State  * inpState,
	OUT ORILIB_t_BitBucket24         * outBuf
	){

	// tail is 6 bits after the end of the frame

	Uint32 tail_start_idx = inpState->len % 24;

	_amem4(outBuf->packedBits) = _amem4_const(inpBuf->packedBits);
	_amem4(outBuf->packedBits) = _clr(
		_amem4_const(outBuf->packedBits),
		31-6-tail_start_idx,
		31-tail_start_idx
	);
}
