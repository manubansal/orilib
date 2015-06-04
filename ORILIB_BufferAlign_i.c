/**
Atomix project, ORILIB_BufferAlign_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_t.h"
#include "ORILIB_BufferAlign_inner.h"

void ORILIB_BufferAlign_i(
		IN  ORILIB_t_Cplx16Buf80 const * const restrict unalignedRawSampleBuf,
		IN  ORILIB_t_AlignState * alignStateInpOut,
		OUT ORILIB_t_Cplx16Buf80 * const restrict alignedRawSampleBuf
		) {

	Cplx16 const * const restrict unaligned_raw_samples = unalignedRawSampleBuf->samples;
	Cplx16 * const restrict aligned_raw_samples = alignedRawSampleBuf->samples;

	ORILIB_BufferAlign_inner(
		unaligned_raw_samples,
		alignStateInpOut,
		aligned_raw_samples
		);
}

