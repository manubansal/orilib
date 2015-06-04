/**
Atomix project, ORILIB_BufferAlignOffset_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_t.h"
#include "ORILIB_BufferAlign_inner.h"

void ORILIB_BufferAlignOffset_i(
		IN  ORILIB_t_Cplx16Buf80 const * const restrict unalignedRawSampleBuf,
		IN  ORILIB_t_AlignState * alignStateInpOut,
		OUT void * const alignedRawSampleBuf,
		CF  ORILIB_t_Offset1 * conf
		) {

	Cplx16 const * const restrict unaligned_raw_samples = unalignedRawSampleBuf->samples;
	Cplx16 * const restrict aligned_raw_samples = 
		(Cplx16 *)((Uint8 *)alignedRawSampleBuf + conf->byteOffset);

	ORILIB_BufferAlign_inner(
		unaligned_raw_samples,
		alignStateInpOut,
		aligned_raw_samples
		);
}


void ORILIB_BufferAlignOffset_i_conf (
		CF  ORILIB_t_Offset1 * conf,
		IN  Uint32 byteOffset
		) {
  conf->byteOffset = byteOffset;
}
