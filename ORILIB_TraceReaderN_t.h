/**
Atomix project, ORILIB_TraceReaderN_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_TRACEREADERN_T_H
#define ORILIB_TRACEREADERN_T_H 

#include <osl/inc/swpform.h>
#include "ORILIB_t.h"

typedef struct {
  Uint32 nextReadOffset __attribute__((aligned(4)));
  Uint32 nWraps; 
  Uint32 nSamplesRead;
  Uint64 lastReadCycle;
} ORILIB_TraceReaderN_t_State;

typedef struct {
  Int16 *sampleTrace;
  Uint32 nSamples;
  Uint32 nSamplesPerSec;
  Uint32 nWrapArounds;
  Uint64 throttleIntervalInCycles;
  Uint32 reportTimestamps;
  Uint32 N;
} ORILIB_TraceReaderN_t_Conf;

#endif /* ORILIB_TRACEREADERN_T_H */
