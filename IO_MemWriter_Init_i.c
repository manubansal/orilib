/**
Atomix project, IO_MemWriter_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include "IO_MemWriter_Init_t.h"

void IO_MemWriter_Init_i(
	OUT IO_t_MemWriter_State * outStateBuf
	) {

  outStateBuf->nextWriteOffset = 0;

  DEBUG(
  LOG_PRINTF("IO_MemWriter_Init: nextWriteOffset = %d\n", outStateBuf->nextWriteOffset);
  )
}
