/**
Atomix project, ORILIB_CNRTracker_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_CNRTRACKER_T_H
#define ORILIB_CNRTRACKER_T_H 

#include "ORILIB_t.h"

typedef struct {
  Uint32 currWindowEnergy[10]	__attribute__((aligned(8)));
  Uint32 lastWriteIndex;
  Uint32 currCNREstimate;
} ORILIB_t_CNRState;

#endif /* ORILIB_CNRTRACKER_T_H */
