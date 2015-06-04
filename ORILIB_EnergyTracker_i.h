/**
Atomix project, ORILIB_EnergyTracker_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_ENERGYTRACKER_I_H 

#include "ORILIB_t.h"

void ORILIB_EnergyTracker_i (
		IN ORILIB_t_Cplx16Buf80 const * const restrict unalignedRawSampleBuf,
		IN ORILIB_t_EnergyState 	* inpEnergyState
		);


void ORILIB_EnergyTracker_computeMultipleWindowEnergy( 
		IN ComplexSamples const * const restrict samples,
		INOUT ORILIB_t_EnergyState * energyState
		);

void ORILIB_EnergyTracker_algo_energyLoop(
		IN 	Cplx16 const sampleBuf[restrict],				
			//new buffer of samples, length SYNC_BUFFER_SIZE_ENERGY

		INOUT 	Uint32 zwinSampleEnergyTermBuf[restrict],

		IN	Uint32 *zwinPositionE2,					
			//next position of the zwinSampleEnergyTerm buffer to be used

		INOUT	Uint32 *currWindowEnergyE2,				
			//as input, energy of the window ending at the last sample

		OUT	Uint32 windowEnergyE1andE2Buf[restrict],		
			//buffer to hold window energy values, length = SYNC_BUFFER_SIZE_ENERGY

		IN	Uint32 *positionE2
		);


#endif /* ORILIB_ENERGYTRACKER_I_H */
