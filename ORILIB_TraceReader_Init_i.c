/**
Atomix project, ORILIB_TraceReader_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include <osl/inc/swpform.h>
#include "ORILIB_TraceReader_Init_t.h"

void ORILIB_TraceReader_Init_i(
	OUT ORILIB_TraceReader_t_State * outStateBuf
	) {

  outStateBuf->nextReadOffset = 0;

  DEBUG(
  LOG_PRINTF("ORILIB_TraceReader_Init: nextReadOffset = %d\n", outStateBuf->nextReadOffset);
  )
}
