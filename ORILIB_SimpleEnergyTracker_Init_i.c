/**
Atomix project, ORILIB_SimpleEnergyTracker_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_t.h"

void ORILIB_SimpleEnergyTracker_Init_i(
	OUT ORILIB_t_SimpleEnergyState * simpleEnergyState
	){

	Uint32 i;

	/*****************************************
	 * Initialization
	 ****************************************/
	simpleEnergyState->zwinPositionE2 = 0;
	simpleEnergyState->currWindowEnergy = 0;

	for (i = 0; i < SYNC_WINDOW_SIZE_ENERGY; i++) {
		simpleEnergyState->zwinSampleEnergyTermBuf[i] = 0;
		simpleEnergyState->windowEnergyBuf[i] = 0;
	}

}
