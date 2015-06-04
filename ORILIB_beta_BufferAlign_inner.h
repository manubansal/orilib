/**
Atomix project, ORILIB_beta_BufferAlign_inner.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_BETA_BUFFERALIGN_INNER_H
#define ORILIB_BETA_BUFFERALIGN_INNER_H 

#include "ORILIB_t.h"


	//ORILIB_t_beta_AlignState * alignStateInpOut,

void ORILIB_beta_BufferAlign_inner(
	Cplx16 const * const restrict unaligned_raw_samples, 
	ORILIB_t_AlignState * alignStateInpOut,
	Cplx16 * const restrict aligned_raw_samples,
	Uint32 nSamplesToSkip	//must be an even number
	);

#endif /* ORILIB_BETA_BUFFERALIGN_INNER_H */
