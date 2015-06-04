/**
Atomix project, ORILIB_EnergyTracker_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_EnergyTracker_i.h"

#if SYNC_BUFFER_SIZE_ENERGY != 80
#error: Need SYNC_BUFFER_SIZE_ENERGY to be 80
#endif



/* ========================================================================== *
 * 			Block implementation function			      *
 * ========================================================================== */
void ORILIB_EnergyTracker_i (
		IN ORILIB_t_Cplx16Buf80 const * const restrict sampleBuf,
		IN ORILIB_t_EnergyState 	* inoutEnergyState
	){

	DEBUG_INFO(LOG_PRINTF("ORILIB_EnergyTracker_i\n");)

	ORILIB_EnergyTracker_computeMultipleWindowEnergy( 
		sampleBuf->samples,
	        inoutEnergyState
		);
}


/* ========================================================================== *
 * 			Internal functions			      	      *
 * ========================================================================== */
void ORILIB_EnergyTracker_computeMultipleWindowEnergy( 
		IN ComplexSamples const * const restrict samples,
		INOUT ORILIB_t_EnergyState * energyState
		) {

    
    Uint32 lo_zwinPositionE2 = _amem4(&energyState->zwinPositionE2);
    Uint32 lo_positionE2 = _amem4(&energyState->positionE2);

    ORILIB_EnergyTracker_algo_energyLoop(
    		IN 	samples,				
			//new buffer of samples, length SYNC_BUFFER_SIZE_ENERGY

    		INOUT 	energyState->zwinSampleEnergyTermBuf,

    		IN	&energyState->zwinPositionE2,					
			//next position of the zwinSampleEnergyTerm buffer to be used

    		INOUT	&energyState->currWindowEnergyE2,				
			//as input, energy of the window ending at the last sample

    		OUT	energyState->windowEnergyE1andE2Buf,		
			//buffer to hold window energy values, length = SYNC_BUFFER_SIZE_ENERGY

    		IN	&energyState->positionE2

    		);

    lo_positionE2 = (lo_positionE2 + SYNC_BUFFER_SIZE_ENERGY) % (SYNC_BUFFER_SIZE_ENERGY + SYNC_WINDOW_SIZE_ENERGY);
    lo_zwinPositionE2 = (lo_zwinPositionE2 + SYNC_BUFFER_SIZE_ENERGY) % SYNC_WINDOW_SIZE_ENERGY;

    _amem4(&energyState->zwinPositionE2) = lo_zwinPositionE2;
    _amem4(&energyState->positionE2) = lo_positionE2;
}


/** This function performs energy computation on the immediately preceding window with
 * respect to the current position in the sample stream. In addition, it also maintains
 * the window before the said window. This is done by keeping a double-length
 * window energy buffer. (The two energy windows are both used in computation
 * of sample correlation metric for the Schmidl-Cox preamble detection
 * algorithm.) Size of both energy windows is SYNC_WINDOW_SIZE_ENERGY.
 *
 * This version of the energy computation algorithm is most useful as the first step
 * for preamble-detection/correlation algorithm, which needs the two energy values. Its
 * output should be fed to an AGC block and the output of the AGC block should be
 * fed into the preamble detector block. Alternately, the AGC maybe by-passed (or the 
 * gain in the AGC block can be hard-coded to 1, for example) if the scale of input
 * samples is externally assured to be occupying the most-significant bits of the 16-bit
 * complex number container that is input for each sample.
 */
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
		) {


	Uint32 i;
	Uint32 zwinSampleEnergyE2;
	Uint32 currSampleEnergy;
	Uint32 lo_currWindowEnergyE2;


	Cplx16U currSample, currSampleConj;

	Uint32 lo_zwinPositionE2, lo_positionE2;		
	//used for E2; values for E1 and AGC are inferred from this.


	lo_currWindowEnergyE2 = _amem4(currWindowEnergyE2);

	lo_zwinPositionE2 = _amem4(zwinPositionE2);
	lo_positionE2 = _amem4(positionE2);


#ifdef DEBUG_MODE
	ASSERT_PTR_ALIGNED(sampleBuf, 8);
	ASSERT_PTR_ALIGNED(zwinSampleEnergyTermBuf, 8);
#endif

	//------------ simpler loop -------------
#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY,SYNC_BUFFER_SIZE_ENERGY, SYNC_BUFFER_SIZE_ENERGY);
	for ( i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i++ ) {

	    zwinSampleEnergyE2 = _amem4(&zwinSampleEnergyTermBuf[lo_zwinPositionE2]);

	    currSample.realimag = _amem4_const(&sampleBuf[i]);		
	    //Q15 (16bit signed samples in (-1.0,1.0)

	    //conjugate of the current sample being processed
	    currSampleConj.cplx16.real = currSample.cplx16.real;
	    currSampleConj.cplx16.imag = -currSample.cplx16.imag;	//conj

	    //current energy term contributed by this sample
	    currSampleEnergy    =   _hill(_cmpy( currSample.realimag, currSampleConj.realimag ));
								    //Q30

	    //bit shift before summation to prevent overflow
	    //currSampleEnergy >>= SYNC_WINDOW_SIZE_ENERGY_N_BITS;	
	    //so this is averaging (Q30 value is the average) 
	    //Q(30-N_BITS)

	    //update the energy moving sum for window E2
	    lo_currWindowEnergyE2 += currSampleEnergy;
	    lo_currWindowEnergyE2 -= zwinSampleEnergyE2;


	    _amem4(&windowEnergyE1andE2Buf[lo_positionE2]) = lo_currWindowEnergyE2;	
	    //Q30 again, since this is a sum over 2^N_BITS values

	    _amem4(&zwinSampleEnergyTermBuf[lo_zwinPositionE2]) = currSampleEnergy;

	    lo_positionE2 = (lo_positionE2 + 1) % (SYNC_BUFFER_SIZE_ENERGY + SYNC_WINDOW_SIZE_ENERGY);
	    lo_zwinPositionE2 = (lo_zwinPositionE2 + 1) % SYNC_WINDOW_SIZE_ENERGY;

	}
	//------------ simpler loop -------------


	_amem4(currWindowEnergyE2) = lo_currWindowEnergyE2;

}

