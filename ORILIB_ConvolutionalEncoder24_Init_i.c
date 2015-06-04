/**
Atomix project, ORILIB_ConvolutionalEncoder24_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include "ORILIB_ConvolutionalEncoder24_Init_t.h"

void ORILIB_ConvolutionalEncoder24_Init_i(
	OUT ORILIB_ConvolutionalEncoder24_t_State * outStateBuf
	) {
  outStateBuf->previousSixInputBits = 0;
}
