/**
Atomix project, ORILIB_beta_BufferAlign_INP96_OUT80_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_t.h"
#include "ORILIB_BufferAlign_inner.h"

void ORILIB_beta_BufferAlign_INP96_OUT80_i(
		IN  ORILIB_t_Cplx16Buf96 const * const restrict unalignedRawSampleBuf,
		IN  ORILIB_t_AlignState * alignStateInpOut,
		OUT ORILIB_t_Cplx16Buf80 * const restrict alignedRawSampleBuf
		) {

	Cplx16 const * const restrict unaligned_raw_samples = unalignedRawSampleBuf->samples;
	Cplx16 * const restrict aligned_raw_samples = alignedRawSampleBuf->samples;

	Uint32 nSamplesToSkip = 16;

	ORILIB_beta_BufferAlign_inner(
		unaligned_raw_samples,
		alignStateInpOut,
		aligned_raw_samples,
		nSamplesToSkip
		);
}

