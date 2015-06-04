/**
Atomix project, ORILIB_BufferAlign_inner.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_BUFFERALIGN_INNER_H 

#include "ORILIB_t.h"

void ORILIB_BufferAlign_inner(
	Cplx16 const * const restrict unaligned_raw_samples, 
	ORILIB_t_AlignState * alignStateInpOut,
	Cplx16 * const restrict aligned_raw_samples
	);

#endif /* ORILIB_BUFFERALIGN_INNER_H */
