/**
Atomix project, ORILIB_AutoGainController_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_t.h"
#include "WIFILIB_util.h"
#include <c6x.h>

#if SYNC_BUFFER_SIZE_ENERGY != 80
#error: Need SYNC_BUFFER_SIZE_ENERGY to be 80
#endif

#define MOD1 (SYNC_BUFFER_SIZE_ENERGY + SYNC_WINDOW_SIZE_ENERGY)
#define MOD2 (SYNC_BUFFER_SIZE_ENERGY)

static void inner(
		IN 		Cplx16 const freshSampleBuf[restrict],
		IN		Uint32 windowEnergyAgcBuf[restrict],
		INOUT		Uint32 zwinSampleEnergyTermBuf_withGain[restrict],
		IN		Uint32 zwinPositionE2,					
		INOUT		Uint32 *currWindowEnergyE2_withGain,
		IN		Uint32 positionE2,
		OUT		Cplx16 raw_samples_with_gain[restrict],
		OUT		Uint32 currAgcGainBuf[restrict],	
		OUT		Uint32 *currAgcGain,
		OUT		Uint32 windowEnergyE1andE2Buf_withGain[restrict]
		) {


    //Uint32 zwinSampleEnergyE2_withGain;
    Uint32 z1;//, z2;
    Uint64 z1z2, z1z12;

    //Uint32 currSampleEnergy_withGain;
    Uint32 e1, e2;
    Uint64 e1e2, e1e12;

    //Uint32 lo_currWindowEnergyE2_withGain;
    Uint32 w0;
    Uint64 w1w2;

    //Cplx16U currSample;
    Uint64 xo1xo2;
    Cplx16U xo1, xo2;

    //Cplx16U currSample_withGain;
    Cplx16U x1, x2;
    Uint64 x1x2;

    Int32 g = 0; //lo_currAgcGain, lo_newAgcGain;
    Int32 n1 = 0, n2 = 0;//, currAvgSampleNorm;
    Int64 z64 = 0;

    //Uint32 lo_currWindowEnergyAgc;
    Uint64 wo1wo2;

    Uint32 lo_zwinPositionE2, lo_positionE2;

    Uint32 ii;

    //lo_currWindowEnergyE2_withGain = _amem4(currWindowEnergyE2_withGain);
    w0 = _amem4(currWindowEnergyE2_withGain);
    w1w2 = _itoll(w0, w0);

    //lo_currAgcGain = *currAgcGain;
    g = *currAgcGain;


    lo_zwinPositionE2 = zwinPositionE2;
    lo_positionE2 = positionE2;

//#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY,SYNC_BUFFER_SIZE_ENERGY, SYNC_BUFFER_SIZE_ENERGY);
#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY/2,SYNC_BUFFER_SIZE_ENERGY/2, SYNC_BUFFER_SIZE_ENERGY/2);
    //for ( ii = 0; ii < SYNC_BUFFER_SIZE_ENERGY; ii++ ) {
    for ( ii = 0; ii < SYNC_BUFFER_SIZE_ENERGY; ii+=2) {
	//currSample.realimag = _amem4_const(&freshSampleBuf[ii]);		
	//Q15 (16bit signed samples in (-1.0,1.0)
	//lo_currWindowEnergyAgc =_amem4(&windowEnergyAgcBuf[ii]);
	//zwinSampleEnergyE2_withGain = _amem4(&zwinSampleEnergyTermBuf_withGain[lo_zwinPositionE2]);
	xo1xo2 = _amem8_const(&freshSampleBuf[ii]);
	wo1wo2 = _amem8_const(&windowEnergyAgcBuf[ii]);
	z1z2 = _amem8(&zwinSampleEnergyTermBuf_withGain[lo_zwinPositionE2]);

#ifdef NO_AGC
	//lo_newAgcGain = 0;
	//lo_currAgcGain = 0;
	//g = 0;
	//currSample_withGain.realimag = currSample.realimag;
	x1x2 = xo1xo2;

	//_amem4(&currAgcGainBuf[ii]) = lo_currAgcGain;
	_amem8(&currAgcGainBuf[ii]) = z64;
#else
	//currAvgSampleNorm = (_norm(lo_currWindowEnergyAgc) >> 1) + SYNC_WINDOW_SIZE_AGC_N_BITS/2;
	n1 = (_norm(_hill(wo1wo2)) >> 1) + SYNC_WINDOW_SIZE_AGC_N_BITS/2;
	n2 = (_norm(_loll(wo1wo2)) >> 1) + SYNC_WINDOW_SIZE_AGC_N_BITS/2;

	//lo_newAgcGain = currAvgSampleNorm - SYNC_AGC_GAIN_SAFETY_MARGIN;
	n1 -= SYNC_AGC_GAIN_SAFETY_MARGIN;
	n2 -= SYNC_AGC_GAIN_SAFETY_MARGIN;

	//lo_currAgcGain = _subabs4(lo_newAgcGain, lo_currAgcGain) >= SYNC_AGC_GAIN_RELOCK_THRESHOLD ?
	//			lo_newAgcGain : lo_currAgcGain;
	//WIFILIB_util_applyGain(&currSample, &currSample_withGain, lo_currAgcGain);
	//_amem4(&currAgcGainBuf[ii]) = lo_currAgcGain;
	//THE FOLLOWING HAS NOT BEEN TESTED!!!
	g = _subabs4(n1, g) >= SYNC_AGC_GAIN_RELOCK_THRESHOLD ?  n1 : g;
	WIFILIB_util_applyGain(&_hill(xo1xo2), &_hill(x1x2), g);
	_amem4(&currAgcGainBuf[ii]) = g;

	g = _subabs4(n2, g) >= SYNC_AGC_GAIN_RELOCK_THRESHOLD ?  n2 : g;
	WIFILIB_util_applyGain(&_loll(xo1xo2), &_loll(x1x2), g);
	_amem4(&currAgcGainBuf[ii + 1]) = g;
#endif

	//currSampleEnergy_withGain = _dotp2(currSample_withGain.realimag, currSample_withGain.realimag);
	x1.realimag = _hill(x1x2);
	x2.realimag = _loll(x1x2);
	e1 = _dotp2(x1.realimag, x1.realimag);
	e2 = _dotp2(x2.realimag, x2.realimag);
	e1e2 = _itoll(e1, e2);

	//currSampleEnergy_withGain >>= SYNC_WINDOW_SIZE_ENERGY__ACCUMULATION_RSHIFT;
	e1e2 = _dshru(e1e2, SYNC_WINDOW_SIZE_ENERGY__ACCUMULATION_RSHIFT);
	e1 = _hill(e1e2);

	//lo_currWindowEnergyE2_withGain -= zwinSampleEnergyE2_withGain;
	//lo_currWindowEnergyE2_withGain += currSampleEnergy_withGain;
	z1 = _hill(z1z2);
	z1z12 = _dadd(z1z2, _itoll(0, z1));
	e1e12 = _dadd(e1e2, _itoll(0, e1));
	w1w2 = _dsub(w1w2, z1z12);
	w1w2 = _dadd(w1w2, e1e12);

	//_amem4(&raw_samples_with_gain[ii]) = currSample_withGain.realimag;
	_amem8(&raw_samples_with_gain[ii]) = x1x2;

	//_amem4(&windowEnergyE1andE2Buf_withGain[lo_positionE2]) = lo_currWindowEnergyE2_withGain;
	_amem8(&windowEnergyE1andE2Buf_withGain[lo_positionE2]) = w1w2;

	//_amem4(&zwinSampleEnergyTermBuf_withGain[lo_zwinPositionE2]) = currSampleEnergy_withGain;
	_amem8(&zwinSampleEnergyTermBuf_withGain[lo_zwinPositionE2]) = e1e2;

	//lo_positionE2 = (lo_positionE2 + 1) % (SYNC_BUFFER_SIZE_ENERGY + SYNC_WINDOW_SIZE_ENERGY);
	//lo_zwinPositionE2 = (lo_zwinPositionE2 + 1) % SYNC_WINDOW_SIZE_ENERGY;

	//lo_positionE2++; lo_zwinPositionE2++;
	lo_positionE2 += 2; lo_zwinPositionE2 += 2;
	lo_positionE2 = lo_positionE2 < MOD1 ? lo_positionE2 : (lo_positionE2 - MOD1);
	lo_zwinPositionE2 = lo_zwinPositionE2 < MOD2 ? lo_zwinPositionE2 : (lo_zwinPositionE2 - MOD2);

	w0 = _loll(w1w2);
	w1w2 = _itoll(w0, w0);

    } 
    //_amem4(currWindowEnergyE2_withGain) = lo_currWindowEnergyE2_withGain;
    //*currAgcGain = lo_currAgcGain;
    _amem4(currWindowEnergyE2_withGain) = w0;
    *currAgcGain = g;

}


//TODO: optimize this loop
static void inner_original(
		IN 		Cplx16 const freshSampleBuf[restrict],
		IN		Uint32 windowEnergyAgcBuf[restrict],
		INOUT		Uint32 zwinSampleEnergyTermBuf_withGain[restrict],
		IN		Uint32 zwinPositionE2,					
		INOUT		Uint32 *currWindowEnergyE2_withGain,
		IN		Uint32 positionE2,
		OUT		Cplx16 raw_samples_with_gain[restrict],
		OUT		Uint32 currAgcGainBuf[restrict],	
		OUT		Uint32 *currAgcGain,
		OUT		Uint32 windowEnergyE1andE2Buf_withGain[restrict]
		) {

    Uint32 zwinSampleEnergyE2_withGain;
    Uint32 currSampleEnergy_withGain;
    Uint32 lo_currWindowEnergyE2_withGain;
    Cplx16U currSample, currSample_withGain;
    Int32 lo_currAgcGain, lo_newAgcGain, currAvgSampleNorm;

    Uint32 lo_currWindowEnergyAgc;

    Uint32 lo_zwinPositionE2, lo_positionE2;

    Uint32 ii;

    lo_currWindowEnergyE2_withGain = _amem4(currWindowEnergyE2_withGain);
    lo_currAgcGain = *currAgcGain;


    lo_zwinPositionE2 = zwinPositionE2;
    lo_positionE2 = positionE2;

#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY,SYNC_BUFFER_SIZE_ENERGY, SYNC_BUFFER_SIZE_ENERGY);
    for ( ii = 0; ii < SYNC_BUFFER_SIZE_ENERGY; ii++ ) {
	currSample.realimag = _amem4_const(&freshSampleBuf[ii]);		
	//Q15 (16bit signed samples in (-1.0,1.0)

	lo_currWindowEnergyAgc =_amem4(&windowEnergyAgcBuf[ii]);

	zwinSampleEnergyE2_withGain = _amem4(&zwinSampleEnergyTermBuf_withGain[lo_zwinPositionE2]);

#ifdef NO_AGC
	lo_newAgcGain = 0;
	lo_currAgcGain = 0;
#else
	currAvgSampleNorm = (_norm(lo_currWindowEnergyAgc) >> 1) + SYNC_WINDOW_SIZE_AGC_N_BITS/2;
	// Average sample scale/norm, expressed as number of
	// redundant sign bits. Thus, if the *average* sample
	// energy is 2^12, we are returning floor((32 - 13)/2)
	// = 9, which is to say that the maximum (average) bit
	// occupancy of samples is 16 - 9 -1 = 6 bits, or that
	// the samples can be safely left-shifted by 9 bits
	// without saturation. If the minimum SNR of operation
	// is 3dB (maximum noise-signal ratio), you expect a
	// standard deviation in sample value equal to (x +/-
	// x/sqrt(2)), so the one-std interval is within 1.7x,
	// and 5sigma interval is within a factor of 4.5, or
	// 2-3 bits.  Thus, instead of shifting left by 9 bits,
	// one can shift by 7 or 6 bits very safely, so a
	// reasonably safe bit margin is 2 bits.

	lo_newAgcGain = currAvgSampleNorm - SYNC_AGC_GAIN_SAFETY_MARGIN;
	lo_currAgcGain = _subabs4(lo_newAgcGain, lo_currAgcGain) >= SYNC_AGC_GAIN_RELOCK_THRESHOLD ?
				lo_newAgcGain : lo_currAgcGain;
#endif

	_amem4(&currAgcGainBuf[ii]) = lo_currAgcGain;

	DEBUG_VERBOSE(
	static far int count = 0;
	LOG_PRINTF("sampleIdx: %6d  agcGain: %d\n", ++count,
			lo_currAgcGain);
	)


	WIFILIB_util_applyGain(&currSample, &currSample_withGain, lo_currAgcGain);

	currSampleEnergy_withGain = _dotp2(currSample_withGain.realimag, currSample_withGain.realimag);

	currSampleEnergy_withGain >>= SYNC_WINDOW_SIZE_ENERGY__ACCUMULATION_RSHIFT;

	lo_currWindowEnergyE2_withGain -= zwinSampleEnergyE2_withGain;
	lo_currWindowEnergyE2_withGain += currSampleEnergy_withGain;

	_amem4(&raw_samples_with_gain[ii]) = currSample_withGain.realimag;
	_amem4(&windowEnergyE1andE2Buf_withGain[lo_positionE2]) = lo_currWindowEnergyE2_withGain;
	_amem4(&zwinSampleEnergyTermBuf_withGain[lo_zwinPositionE2]) = currSampleEnergy_withGain;

	lo_positionE2 = (lo_positionE2 + 1) % (SYNC_BUFFER_SIZE_ENERGY + SYNC_WINDOW_SIZE_ENERGY);
	lo_zwinPositionE2 = (lo_zwinPositionE2 + 1) % SYNC_WINDOW_SIZE_ENERGY;

    } 
    _amem4(currWindowEnergyE2_withGain) = lo_currWindowEnergyE2_withGain;
    *currAgcGain = lo_currAgcGain;

}

/* ========================================================================== *
 * 			Block implementation function			      *
 * ========================================================================== */
void ORILIB_AutoGainController_i (
		IN  ORILIB_t_Cplx16Buf80 const 	* const restrict unalignedRawSampleBuf,
		IN  ORILIB_t_SimpleEnergyState 	* inoutSimpleEnergyState,
		IN  ORILIB_t_GainState 		* inoutGainState,
		IN  ORILIB_t_GainStateAux 	* inoutGainStateAux,
		OUT ORILIB_t_Cplx16Buf80 	* const restrict unalignedRawSampleBufWithGain
	){


    ComplexSamples const * raw_samples = unalignedRawSampleBuf->samples;
    ComplexSamples * raw_samples_with_gain = unalignedRawSampleBufWithGain->samples;
    ORILIB_t_SimpleEnergyState * simpleEnergyState = inoutSimpleEnergyState;
    ORILIB_t_GainState * gainState = inoutGainState;
    ORILIB_t_GainStateAux * gainStateAux = inoutGainStateAux;


	      //inner_original(
	      inner(
    		IN 	raw_samples,				
			//new buffer of samples, length SYNC_BUFFER_SIZE_ENERGY

    		IN	simpleEnergyState->windowEnergyBuf,	


    		INOUT	gainState->zwinSampleEnergyTermBuf_withGain,

    		IN	gainState->zwinPositionE2,					
			//next position of the zwinSampleEnergyTerm buffer to be used

    		INOUT	&gainState->currWindowEnergyE2_withGain,


    		IN	gainState->positionE2,

    		OUT	raw_samples_with_gain,

    		//-- the following is for debug only --
    		OUT	gainState->currAgcGainBuf,	
			//buffer to hold norms of energy values - used for gain control

    		OUT	&gainStateAux->currAgcGain,

    		OUT	gainStateAux->windowEnergyE1andE2Buf_withGain
			//buffer to hold window energy values, length = SYNC_BUFFER_SIZE_ENERGY
    		);

    gainState->positionE2 = 
    	(gainState->positionE2 + SYNC_BUFFER_SIZE_ENERGY) % (SYNC_BUFFER_SIZE_ENERGY + SYNC_WINDOW_SIZE_ENERGY);
    gainState->zwinPositionE2 = 
    	(gainState->zwinPositionE2 + SYNC_BUFFER_SIZE_ENERGY) % SYNC_WINDOW_SIZE_ENERGY;
}


