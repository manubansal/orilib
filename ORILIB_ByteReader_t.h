/**
Atomix project, ORILIB_ByteReader_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef __ORILIB_BYTEREADER_T_H__
#define __ORILIB_BYTEREADER_T_H__

#include <osl/inc/swpform.h>
#include "ORILIB_t.h"

typedef struct {
  Uint32 nextReadOffset __attribute__((aligned(4)));
} ORILIB_ByteReader_t_State;

typedef struct {
  Uint8* bytes;
  Uint32 nBytes;
  Uint32 nBytesPerSymbol;
  Uint32 nWrapArounds;
} ORILIB_ByteReader_t_Conf;

#endif
