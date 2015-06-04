/**
Atomix project, ORILIB_TraceReader80_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#ifndef __ORILIB_TRACEREADER80_T_H__
#define __ORILIB_TRACEREADER80_T_H__

#include <osl/inc/swpform.h>
#include "ORILIB_t.h"

typedef struct {
  Uint32 nextReadOffset __attribute__((aligned(4)));
  Uint32 nWraps; 
  Uint32 nSamplesRead;
  Uint64 lastReadCycle;
} ORILIB_TraceReader80_t_State;

typedef struct {
  Int16 *sampleTrace;
  Uint32 nSamples;
  Uint32 nSamplesPerSec;
  Uint32 nWrapArounds;
  Uint64 throttleIntervalInCycles;
  Uint32 reportTimestamps;
} ORILIB_TraceReader80_t_Conf;

#endif //__ORILIB_TRACEREADER80_T_H__
