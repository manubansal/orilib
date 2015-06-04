/**
Atomix project, ORILIB_BufferAlign_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_t.h"

void ORILIB_BufferAlign_Init_i(
	OUT ORILIB_t_AlignState * outAlignState
	) {
	outAlignState->nAlignedSamplesAlreadyFilled = 0;
	outAlignState->nAlignedSamplesAvailable = 0;

	outAlignState->uaks1 = 1 * SYNC_BUFFER_SIZE_ENERGY;
	outAlignState->uaks2 = 2 * SYNC_BUFFER_SIZE_ENERGY;
}
