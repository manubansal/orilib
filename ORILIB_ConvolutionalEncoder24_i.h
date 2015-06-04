/**
Atomix project, ORILIB_ConvolutionalEncoder24_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_CONVOLUTIONALENCODER24_I_H_

#include <osl/inc/swpform.h>
#include "ORILIB_ConvolutionalEncoder24_t.h"

void ORILIB_ConvolutionalEncoder24_i (
      IN 	ORILIB_t_BitBucket24 * inpBitsBuf,
      IN  ORILIB_ConvolutionalEncoder24_t_State * inpStateBuf,
      OUT ORILIB_t_BitBucket24 * outEncodedBitsA,
      OUT ORILIB_t_BitBucket24 * outEncodedBitsB,
      OUT ORILIB_ConvolutionalEncoder24_t_State * outStateBuf
      );

#endif

