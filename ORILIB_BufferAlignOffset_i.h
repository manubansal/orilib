/**
Atomix project, ORILIB_BufferAlignOffset_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_BUFFERALIGNOFFSET_I_H
#define ORILIB_BUFFERALIGNOFFSET_I_H 

#include "ORILIB_t.h"

void ORILIB_BufferAlignOffset_i(
		IN  ORILIB_t_Cplx16Buf80 const * const restrict unalignedRawSampleBuf,
		IN  ORILIB_t_AlignState * alignStateInpOut,
		OUT void * const alignedRawSampleBuf,
		CF  ORILIB_t_Offset1 * conf
		);

void ORILIB_BufferAlignOffset_i_conf (
		CF  ORILIB_t_Offset1 * conf,
		IN  Uint32 byteOffset
		);

#endif /* ORILIB_BUFFERALIGNOFFSET_I_H */
