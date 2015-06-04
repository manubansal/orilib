/**
Atomix project, ORILIB_TraceReader80_ThrottleReinit_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include "ORILIB_TraceReader80_ThrottleReinit_t.h"

void ORILIB_TraceReader80_ThrottleReinit_i(
	IN  ORILIB_TraceReader80_t_State * inpStateBuf
	) {
	//inout, wire with +

  inpStateBuf->lastReadCycle = 0;

  DEBUG(
  LOG_PRINTF("ORILIB_TraceReader80_ThrottleReinit: lastReadCycle = 0\n");
  )
}
