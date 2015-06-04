/**
Atomix project, ORILIB_BufferAlign_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_BUFFERALIGN_I_H 

#include "ORILIB_t.h"

void ORILIB_BufferAlign_i(
		IN  ORILIB_t_Cplx16Buf80 const * const restrict unalignedRawSampleBuf,
		IN  ORILIB_t_AlignState * alignStateInpOut,
		OUT ORILIB_t_Cplx16Buf80 * const restrict alignedRawSampleBuf
		);

#endif /* ORILIB_BUFFERALIGN_I_H */
