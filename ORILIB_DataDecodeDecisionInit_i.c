/**
Atomix project, ORILIB_DataDecodeDecisionInit_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include "ORILIB_DataDecodeDecisionInit_t.h"

void ORILIB_DataDecodeDecisionInit_i (
	OUT ORILIB_t_DataDecodeState *bOutDecodeState
	) {



  bOutDecodeState->dataDecodeMode = DATA_DECODE_STATE_INIT;

  //DEBUG_VERBOSE(
  //LOG_PRINTF("ORILIB_DataDecodeDecisionInit: currState: %d\n", 
  //	bOutDecodeState->dataDecodeMode);
  //)
}
