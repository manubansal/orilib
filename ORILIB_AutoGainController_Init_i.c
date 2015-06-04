/**
Atomix project, ORILIB_AutoGainController_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_t.h"


void ORILIB_AutoGainController_Init_i(
	OUT ORILIB_t_GainState * gainState,
	OUT ORILIB_t_GainStateAux * gainStateAux
	){

	Uint32 i;

	/*****************************************
	 * Initialization
	 ****************************************/
	gainState->zwinPositionE2 = 0;
	gainState->positionE2 = 0;

	gainState->currWindowEnergyE2_withGain = 0;
	gainStateAux->currAgcGain = 0;


	for (i = 0; i < SYNC_WINDOW_SIZE_ENERGY; i++) {
		gainStateAux->windowEnergyE1andE2Buf_withGain[i] = 0;
		gainStateAux->windowEnergyE1andE2Buf_withGain[i + SYNC_BUFFER_SIZE_ENERGY] = 0;


		gainState->currAgcGainBuf[i] = 0;

		gainState->zwinSampleEnergyTermBuf_withGain[i] = 0;
	}
}
