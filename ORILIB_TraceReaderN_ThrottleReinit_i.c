/**
Atomix project, ORILIB_TraceReaderN_ThrottleReinit_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include "ORILIB_TraceReaderN_ThrottleReinit_t.h"

void ORILIB_TraceReaderN_ThrottleReinit_i(
	IN  ORILIB_TraceReaderN_t_State * inpStateBuf
	) {
	//inout, wire with +

  inpStateBuf->lastReadCycle = 0;

  DEBUG(
  LOG_PRINTF("ORILIB_TraceReaderN_ThrottleReinit: lastReadCycle = 0\n");
  )
}
