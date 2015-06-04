/**
Atomix project, ORILIB_CNRTrackerIN_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_CNRTrackerIN_t.h"


static Uint32 compute_energy(
    IN 	Cplx16 const freshSampleBuf[restrict]
    );

//IN   ORILIB_t_SimpleEnergyState * simpleEnergyState,
void ORILIB_CNRTrackerIN_i(
  IN  ORILIB_t_Cplx16Buf80 const 	* const restrict unalignedRawSampleBuf,
  IN  ORILIB_t_CNRState * cnrState
  ) {
  //cnrState is meant to be used as an in-out buffer

//typedef struct {
//  Uint32 currWindowEnergy[10]	__attribute__((aligned(8)));
//  Uint32 lastWriteIndex;
//  Uint32 currCNREstimate;
//} ORILIB_t_CNRState;


  Uint32 i = cnrState->lastWriteIndex;
  //Uint32 energy = simpleEnergyState->currWindowEnergy;
  Uint32 energy = compute_energy(unalignedRawSampleBuf);
  i = (i+1)%10;
  cnrState->currWindowEnergy[i] = energy;
  cnrState->lastWriteIndex = i;
  //Uint32 j = (i + 6) % 10;		//effective i-4
  Uint32 j = (i + 5) % 10;		//effective i-5, so look
  					//at the energy of the sample
					//buffer 5 buffers back 
  Uint32 prevEnergy = cnrState->currWindowEnergy[j];
  double n, d, cnr, cnr_dB;
  n = (double)energy;
  d = (double)prevEnergy;
  if (d > 0) {
    cnr = d > 0 ? n/d : 0;
    //cnr_dB = 10 * log10(cnr);
    cnr_dB = 0;
  }
  else {
    cnr = 0;
    cnr_dB = 0;
  }
  cnrState->currCNREstimate = (Uint32)(cnr * 1e4);
  DEBUG_INFO(
  printf("%d, old energy: %d, new energy: %d, cnr = %f, cnr_dB = %f\n", 
  	i, prevEnergy, energy, cnr, cnr_dB);
  )
}




static Uint32 compute_energy(
		IN 	Cplx16 const freshSampleBuf[restrict]
			//new buffer of samples, length SYNC_BUFFER_SIZE_ENERGY
	){

	Uint32 i;
	Cplx16U currSample, currSampleConj; 
	Uint32 currSampleEnergy;
	Uint32 lo_currWindowEnergyE2 = 0;

#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY,SYNC_BUFFER_SIZE_ENERGY, SYNC_BUFFER_SIZE_ENERGY);
	for ( i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i++ ) {
//	    //this assumes that SYNC_WINDOW_SIZE_ENERGY >= SYNC_WINDOW_SIZE_AGC

	    currSample.realimag = _amem4_const(&freshSampleBuf[i]);		
	    //Q15 (16bit signed samples in (-1.0,1.0)

	    //conjugate of the current sample being processed
	    currSampleConj.cplx16.real = currSample.cplx16.real;
	    currSampleConj.cplx16.imag = -currSample.cplx16.imag;	//conj

	    //current energy term contributed by this sample
	    currSampleEnergy    =   _hill(_cmpy( currSample.realimag, currSampleConj.realimag ));
								    //Q30


	    currSampleEnergy    >>= 
		    (SYNC_WINDOW_SIZE_ENERGY__ACCUMULATION_RSHIFT);

	    //update the energy moving sum for window 
	    lo_currWindowEnergyE2 += currSampleEnergy;

	}


	return lo_currWindowEnergyE2;

}

