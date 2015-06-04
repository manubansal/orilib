/**
Atomix project, ORILIB_TraceReader80_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include <osl/inc/swpform.h>
#include "ORILIB_TraceReader80_Init_t.h"

void ORILIB_TraceReader80_Init_i(
	OUT ORILIB_TraceReader80_t_State * outStateBuf
	) {

  outStateBuf->nextReadOffset = 0;
  outStateBuf->nWraps = 0;
  outStateBuf->nSamplesRead = 0;
  outStateBuf->lastReadCycle = 0;

  DEBUG(
  LOG_PRINTF("ORILIB_TraceReader80_Init: nextReadOffset = %d\n", outStateBuf->nextReadOffset);
  )
}
