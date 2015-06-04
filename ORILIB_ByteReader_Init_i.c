/**
Atomix project, ORILIB_ByteReader_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include "ORILIB_ByteReader_Init_t.h"

void ORILIB_ByteReader_Init_i(
	OUT ORILIB_ByteReader_t_State * outStateBuf
	) {

  outStateBuf->nextReadOffset = 0;

  DEBUG_VERBOSE(
  LOG_PRINTF("ORILIB_ByteReader_Init: nextReadOffset = %d\n", outStateBuf->nextReadOffset);
  )
}
