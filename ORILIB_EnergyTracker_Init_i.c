/**
Atomix project, ORILIB_EnergyTracker_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

void ORILIB_EnergyTracker_Init_i(
	OUT ORILIB_t_EnergyState * energyState
	){

	/*****************************************
	 * Initialization
	 ****************************************/
	//zero-initialize buffers
	energyState->zwinPositionE2 = 0;
	energyState->currWindowEnergyE2 = 0;
	energyState->positionE2 = 0;

	Uint32 i;

	for (i = 0; i < SYNC_WINDOW_SIZE_ENERGY; i++) {
		energyState->windowEnergyE1andE2Buf[i] = 0;
		energyState->windowEnergyE1andE2Buf[i + SYNC_BUFFER_SIZE_ENERGY] = 0;
		energyState->zwinSampleEnergyTermBuf[i] = 0;
	}
}
