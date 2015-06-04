/**
Atomix project, ORILIB_SimpleEnergyTracker_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "WIFILIB_util.h"
#include <c6x.h>

#if SYNC_BUFFER_SIZE_ENERGY != 80
#error: Need SYNC_BUFFER_SIZE_ENERGY to be 80
#endif

static void inner(
		IN 	Cplx16 const freshSampleBuf[restrict],				
		IN 	Uint32 zwinSampleEnergyTermBuf[restrict],
		IN	Uint32 zwinPositionE2,					
		INOUT	Uint32 *currWindowEnergyE2,
		OUT	Uint32 windowEnergyE1andE2Buf[restrict]	//only for debug
	){

	Uint32 i;
	//Cplx16U currSample, currSampleConj; 
	Uint64 x1x2;

	//Uint32 zwinSampleEnergy, currSampleEnergy;
	Uint64 d1d2, e1e2;
	Uint64 d1d12, e1e12;
	__x128_t e1e2_32;

	Uint32 w0; //lo_currWindowEnergyE2;
	Uint64 w1w2;

	Uint32 lo_zwinPositionE2;

	lo_zwinPositionE2 = zwinPositionE2;

	//lo_currWindowEnergyE2 = _amem4(currWindowEnergyE2);
	w0 = _amem4(currWindowEnergyE2);
	w1w2 = _itoll(w0, w0);


//#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY,SYNC_BUFFER_SIZE_ENERGY, SYNC_BUFFER_SIZE_ENERGY);
#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY/2,SYNC_BUFFER_SIZE_ENERGY/2, SYNC_BUFFER_SIZE_ENERGY/2);
	for ( i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i+=2 ) {
	    //zwinSampleEnergy = _amem4(&zwinSampleEnergyTermBuf[lo_zwinPositionE2]);
	    d1d2 = _amem8(&zwinSampleEnergyTermBuf[lo_zwinPositionE2]);

	    //Q15 (16bit signed samples in (-1.0,1.0)
	    //currSample.realimag = _amem4_const(&freshSampleBuf[i]);		
	    x1x2 = _amem8_const(&freshSampleBuf[i]);		

	    //conjugate of the current sample being processed
	    //currSampleConj.cplx16.real = currSample.cplx16.real;
	    //currSampleConj.cplx16.imag = -currSample.cplx16.imag;	//conj

	    //current energy term contributed by this sample
	    //currSampleEnergy    =   
	    //	_hill(_cmpy( currSample.realimag, currSampleConj.realimag )); //Q30
	    e1e2_32 = _dccmpy(x1x2, x1x2);
	    e1e2 = _itoll(_get32_128(e1e2_32, 0), _get32_128(e1e2_32, 2));


	    //currSampleEnergy    >>= 
	    //	(SYNC_WINDOW_SIZE_ENERGY__ACCUMULATION_RSHIFT);
	    e1e2 = _dshr(e1e2, SYNC_WINDOW_SIZE_ENERGY__ACCUMULATION_RSHIFT);

	    //update the energy moving sum for window 
	    //lo_currWindowEnergyE2 += currSampleEnergy;
	    //lo_currWindowEnergyE2 -= zwinSampleEnergy;
	    d1d12 = _dadd(d1d2, _itoll(0, _hill(d1d2)));
	    e1e12 = _dadd(e1e2, _itoll(0, _hill(e1e2)));
	    w1w2 = _dsub(w1w2, d1d12);
	    w1w2 = _dadd(w1w2, e1e12);


	    //_amem4(&windowEnergyE1andE2Buf[i]) = lo_currWindowEnergyE2;
	    //_amem4(&zwinSampleEnergyTermBuf[lo_zwinPositionE2]) = currSampleEnergy;
	    _amem8(&windowEnergyE1andE2Buf[i]) = w1w2;
	    _amem8(&zwinSampleEnergyTermBuf[lo_zwinPositionE2]) = e1e2;

	    //lo_zwinPositionE2 = (lo_zwinPositionE2 + 1) % SYNC_WINDOW_SIZE_ENERGY;
	    lo_zwinPositionE2 = (lo_zwinPositionE2 + 2) % SYNC_WINDOW_SIZE_ENERGY;

	    w0 = _loll(w1w2);
	    w1w2 = _itoll(w0, w0);

	}

	//_amem4(currWindowEnergyE2) = lo_currWindowEnergyE2;
	_amem4(currWindowEnergyE2) = w0;
}


static void inner_unoptimized(
		IN 	Cplx16 const freshSampleBuf[restrict],				
			//new buffer of samples, length SYNC_BUFFER_SIZE_ENERGY

		IN 	Uint32 zwinSampleEnergyTermBuf[restrict],

		IN	Uint32 zwinPositionE2,					
			//next position of the zwinSampleEnergyTerm buffer to be used

		INOUT	Uint32 *currWindowEnergyE2,

		OUT	Uint32 windowEnergyE1andE2Buf[restrict]	//only for debug
	){

	Uint32 i;
	Cplx16U currSample, currSampleConj; 
	Uint32 zwinSampleEnergy;
	Uint32 currSampleEnergy;
	Uint32 lo_currWindowEnergyE2;

	Uint32 lo_zwinPositionE2;

	lo_currWindowEnergyE2 = _amem4(currWindowEnergyE2);
	lo_zwinPositionE2 = zwinPositionE2;

#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY,SYNC_BUFFER_SIZE_ENERGY, SYNC_BUFFER_SIZE_ENERGY);
	for ( i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i++ ) {
//	    //this assumes that SYNC_WINDOW_SIZE_ENERGY >= SYNC_WINDOW_SIZE_AGC
	    zwinSampleEnergy = _amem4(&zwinSampleEnergyTermBuf[lo_zwinPositionE2]);

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
	    lo_currWindowEnergyE2 -= zwinSampleEnergy;


	    _amem4(&windowEnergyE1andE2Buf[i]) = lo_currWindowEnergyE2;


	    _amem4(&zwinSampleEnergyTermBuf[lo_zwinPositionE2]) = currSampleEnergy;

	    lo_zwinPositionE2 = (lo_zwinPositionE2 + 1) % SYNC_WINDOW_SIZE_ENERGY;



	}




	_amem4(currWindowEnergyE2) = lo_currWindowEnergyE2;

}


void ORILIB_SimpleEnergyTracker_i(
		IN  ORILIB_t_Cplx16Buf80 const 	* const restrict unalignedRawSampleBuf,
		IN  ORILIB_t_SimpleEnergyState * simpleEnergyState
		) {

      ComplexSamples const * raw_samples = unalignedRawSampleBuf->samples;

      inner(
	IN 	raw_samples,				
		//new buffer of samples, length SYNC_BUFFER_SIZE_ENERGY

	IN 	simpleEnergyState->zwinSampleEnergyTermBuf,

	IN	simpleEnergyState->zwinPositionE2,					
		//next position of the zwinSampleEnergyTerm buffer to be used

	INOUT	&simpleEnergyState->currWindowEnergy,

	OUT	simpleEnergyState->windowEnergyBuf
	);

    simpleEnergyState->zwinPositionE2 = 
    	(simpleEnergyState->zwinPositionE2 + SYNC_BUFFER_SIZE_ENERGY) % SYNC_WINDOW_SIZE_ENERGY;
}
