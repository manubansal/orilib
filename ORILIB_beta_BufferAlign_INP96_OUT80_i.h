/**
Atomix project, ORILIB_beta_BufferAlign_INP96_OUT80_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_BETA_BUFFERALIGN_INP96_OUT80_I_H
#define ORILIB_BETA_BUFFERALIGN_INP96_OUT80_I_H 

#include "ORILIB_t.h"

void ORILIB_beta_BufferAlign_INP96_OUT80_i(
		IN  ORILIB_t_Cplx16Buf96 const * const restrict unalignedRawSampleBuf,
		IN  ORILIB_t_AlignState * alignStateInpOut,
		OUT ORILIB_t_Cplx16Buf80 * const restrict alignedRawSampleBuf
		);

#endif /* ORILIB_BETA_BUFFERALIGN_INP96_OUT80_I_H */
