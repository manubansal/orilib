/**
Atomix project, ORILIB_AckPacker_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_ACKPACKER_I_H_ 

#include "ORILIB_AckPacker_t.h"
#include "ORILIB_TailZeroer24_t.h"

void ORILIB_AckPacker_i(
            IN   ORILIB_t_ByteBuf8           * inpMacBuf,
            OUT  ORILIB_t_BitBucket256       * outBitsBuf,
						OUT  ORILIB_TailZeroer24_t_State * outTailZState
            );

#endif
