/**
Atomix project, ORILIB_SimpleEnergyTracker_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_SIMPLEENERGYTRACKER_I_H
#define ORILIB_SIMPLEENERGYTRACKER_I_H 

#include "ORILIB_t.h"

void ORILIB_SimpleEnergyTracker_i(
		IN  ORILIB_t_Cplx16Buf80 const 	* const restrict unalignedRawSampleBuf,
		IN  ORILIB_t_SimpleEnergyState * simpleEnergyState
		);

#endif /* ORILIB_SIMPLEENERGYTRACKER_I_H */
