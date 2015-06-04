/**
Atomix project, IO_MemWriter_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef __IO_MEMWRITER_T_H__
#define __IO_MEMWRITER_T_H__

#include <osl/inc/swpform.h>
#include "ORILIB_t.h"

typedef struct {
  Uint32 nextWriteOffset __attribute__((aligned(4)));
} IO_t_MemWriter_State;

typedef struct {
  Uint8* bytes;
  Uint32 nBytes;
  Uint32 nBytesPerSymbol;
  Uint32 nWrapArounds;
} IO_t_MemWriter_Conf;

#endif
