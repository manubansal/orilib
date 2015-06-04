/**
Atomix project, ORILIB_beta_BufferAlignOffset_INP96_OUT80_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_BETA_BUFFERALIGNOFFSET_INP96_OUT80_I_H
#define ORILIB_BETA_BUFFERALIGNOFFSET_INP96_OUT80_I_H 

#include "ORILIB_t.h"

void ORILIB_beta_BufferAlignOffset_INP96_OUT80_i(
		IN  ORILIB_t_Cplx16Buf96 const * const restrict unalignedRawSampleBuf,
		IN  ORILIB_t_AlignState * alignStateInpOut,
		OUT void * const alignedRawSampleBuf,
		CF  ORILIB_t_beta_Offset1 * conf
		);

void ORILIB_beta_BufferAlignOffset_INP96_OUT80_i_conf (
		CF  ORILIB_t_beta_Offset1 * conf,
		IN  Uint32 byteOffset,
		IN  Uint32 nSamplesToSkip
		);

#endif /* ORILIB_BETA_BUFFERALIGNOFFSET_INP96_OUT80_I_H */
