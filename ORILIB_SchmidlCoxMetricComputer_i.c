/**
Atomix project, ORILIB_SchmidlCoxMetricComputer_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_t.h"
#include <c6x.h>

#if SYNC_BUFFER_SIZE_ENERGY != 80
#error: Need SYNC_BUFFER_SIZE_ENERGY to be 80
#endif


static void inner(
		IN	Uint32 windowEnergyE1andE2Buf[restrict],			
		INOUT	Uint32 *positionNextE2Value,
		IN	Cplx16	winCorrBuf[restrict],
		INOUT	Uint32	debug_magNumerBuf[restrict],
		INOUT	Uint32	debug_magDenomBuf[restrict],
		INOUT	Uint32	debug_metricBuf[restrict]
		){

    Uint32 i;
    Uint32 idxE2;
    idxE2 = _amem4(positionNextE2Value);

#define MOD (SYNC_WINDOW_SIZE_ENERGY + SYNC_BUFFER_SIZE_ENERGY)
#define DELTA ((SYNC_BUFFER_SIZE_ENERGY + SYNC_WINDOW_SIZE_ENERGY) - SYNC_WINDOW_SIZE_ENERGY)

#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY/2,SYNC_BUFFER_SIZE_ENERGY/2, SYNC_BUFFER_SIZE_ENERGY/2);
    for ( i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i+=2 ) {
	Uint64 M1M2;

	Uint64 E1E2, A1A2;
	Uint64 e1e2, d1d2, w1w2;

	float Af, Ef;
	__float2_t M1M2f;
	Uint32 idxE1;


	//---------------------load window energy values ----------------------
	idxE1 = (idxE2 + DELTA) % MOD;
	d1d2 = _amem8(&windowEnergyE1andE2Buf[idxE1]); //E1
	e1e2 = _amem8(&windowEnergyE1andE2Buf[idxE2]); //E2
	w1w2 = _amem8(&winCorrBuf[i]);

	//---------------------- begin metric computation ----------------------
	A1A2 = _itoll(_dotp2(_hill(w1w2), _hill(w1w2)),_dotp2(_loll(w1w2), _loll(w1w2)));
	E1E2 = _itoll(_hill(_mpy32u(_hill(d1d2), _hill(e1e2))), _hill(_mpy32u(_loll(d1d2), _loll(e1e2))));

	E1E2 = _dadd(E1E2, 0x0000000100000001);

	M1M2f = _ftof2(
	(float)(_hill(A1A2)) * _rcpsp((float)(_hill(E1E2))) * 1024,
	(float)(_loll(A1A2)) * _rcpsp((float)(_loll(E1E2))) * 1024
	);

	M1M2 = _dspint(M1M2f);
	//---------------------- done metric computation ----------------------

	_amem8(&debug_magNumerBuf[i]) = A1A2;
	_amem8(&debug_magDenomBuf[i]) = E1E2;
	_amem8(&debug_metricBuf[i]) = M1M2;

	idxE2 = (idxE2 + 2) % MOD;
    }

    _amem4(positionNextE2Value) = idxE2;
}


static void inner_unoptimized(
		//-- energy-only related fields --
		IN	Uint32 windowEnergyE1andE2Buf[restrict],			
		//buffer to hold window energy values, length = SYNC_BUFFER_SIZE_ENERGY
		INOUT	Uint32 *positionNextE2Value,


		//-- correlation computation specific fields --
		IN	Cplx16	winCorrBuf[restrict],

		//-- for debugging --
		//INOUT	float	debug_metricFBuf[restrict],

		//-- not just for debugging --
		INOUT	Uint32	debug_magNumerBuf[restrict],

		//-- not just for debugging --
		INOUT	Uint32	debug_magDenomBuf[restrict],

		//-- not just for debug --
		INOUT	Uint32	debug_metricBuf[restrict]
		){


    Uint32 i;
    Uint32 	idxE2;
    idxE2 = _amem4(positionNextE2Value);

#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY,SYNC_BUFFER_SIZE_ENERGY, SYNC_BUFFER_SIZE_ENERGY);
    for ( i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i++ ) {
	Uint32 currMetric;
	Uint32 magESqSqTmp, magASqSqTmp;
	Uint32 lo_currWindowEnergy;
	Uint32 lo_zwinWindowEnergy;
	Cplx16U lo_currWinCorr;

	float currMetricF = 0.0f;
	float magESqSqTmpF = 0.0f;
	Uint32 idxE1;

	lo_currWinCorr.realimag = _amem4(&winCorrBuf[i]);


	//---------------------load window energy values ----------------------
	//E2
	lo_currWindowEnergy = _amem4(&windowEnergyE1andE2Buf[idxE2]);

	//E1
	idxE1 = (idxE2 +
	      ((SYNC_BUFFER_SIZE_ENERGY + SYNC_WINDOW_SIZE_ENERGY)
			      - SYNC_WINDOW_SIZE_ENERGY))	
			      //the index we want modulo equivalent positive index
			      % (SYNC_WINDOW_SIZE_ENERGY + SYNC_BUFFER_SIZE_ENERGY);		
			      //modulo operation for true index
	lo_zwinWindowEnergy = _amem4(&windowEnergyE1andE2Buf[idxE1]);


	//---------------------- begin metric computation ----------------------

	//compute magnitude square of moving correlation sum (|P(k)|^2)
	//magASqSqTmp = (Uint64)_mpy32ll(currWinCorr.cplx32.real, currWinCorr.cplx32.real);
	//magASqSqTmp+= (Uint64)_mpy32ll(currWinCorr.cplx32.imag, currWinCorr.cplx32.imag);
	magASqSqTmp = _dotp2(lo_currWinCorr.realimag, lo_currWinCorr.realimag);
	//magASqSqTmp <<= 1;
	//the extra one bit in shift is to account for one bit left-shift by _smpy32 since
	//_dotp2 does not do that one bit shift to the left, hence magESqSqTmp and magASqSqTmp
	//end up differing by 1 bit in scale.


	//compute square of moving sum energy value (R(k)^2) = E1 * E2 = currEnergy * zwinEnergy
	//magESqSqTmp = _smpy32(lo_currWindowEnergy, lo_zwinWindowEnergy);
	//The above statement is computing "incorrect" result
	//in some cases. Specifically, I'm seeing 4294765454 as output
	//when the input is (2157956077, 202831). The problem is that the
	//input quantities are unsigned ints while the instruction multiplies
	//them as signed ints.
	magESqSqTmp = _hill(_mpy32u(lo_currWindowEnergy, lo_zwinWindowEnergy));

	//ok fine, let's also compute the metric M(k) = |P(k)|^2/(R(k))^2
	//normMagESq = _norm(_hill(magESqSqTmp));
	//mags32 = _itoll(_hill(magESqSqTmp << normMagESq), _hill(magASqSqTmp << normMagESq));
	//magsF = _dintspu(mags32);
	//currMetricF = magESqSqTmp > 0 ? _rcpsp(_hif2(magsF)) : 0;
	//currMetricF *= _lof2(magsF);

	//magESqSqTmpF = _itof(magESqSqTmp);
	//currMetricF = magESqSqTmp > 0 ? _rcpsp(magESqSqTmpF) : _itof(0);
	//currMetricF *= _itof(magASqSqTmp);
	//currMetric = _ftoi(currMetricF);

	//version 1
	//magESqSqTmpF = (float)(magESqSqTmp);
	//currMetricF = magESqSqTmp > 0 ? _rcpsp(magESqSqTmpF) : 0.0f;
	//currMetricF = magESqSqTmp > 0 ? currMetricF * (float)(magASqSqTmp) : 0.0f;
	////if (magESqSqTmp > 0)
	////  LOG_PRINTF("found: %d\n", magESqSqTmp);
	//currMetric = (Uint32)(currMetricF * 1024);	
	//so that we keep 10-bit of precision of the metric, which is between 0
	//and 1.

	//version 2
	magESqSqTmp++;	
	//ensures that this term is always >= 1, and it's insigificant amount
	//of error we are introducing. this saves us from comparison before
	//reciprocating.
	magESqSqTmpF = (float)(magESqSqTmp);
	currMetricF = _rcpsp(magESqSqTmpF);
	currMetricF = currMetricF * (float)(magASqSqTmp);


	//currMetric = (Uint32)(currMetricF * 1024);	
	//so that we keep 10-bit of precision of the metric,
	currMetric = _spint(currMetricF * 1024);

	DEBUG_VERBOSE(
	static far int count = 0;
	LOG_PRINTF("sampleIdx: %6d  magA: %12u, magE: %12u, currMetricF: %f, currMetric: %5u\n", ++count,
			magASqSqTmp, magESqSqTmp, currMetricF, currMetric);
	)


	//so that we keep 10-bit of precision of the metric,

	//This can go into Ripley's Believe It or Not. Changing (Uint32) to
	//_spint brought down the cycle count of this loop from 7000 cycles to
	//450 cycles.

	//Explanation: Using the first version (natural C) makes the compiler
	//use the run-time support function __c6xabi_fixfu to covert float to
	//unsigned integer. Not only is this function heavy, as the compiler
	//documentation says, the compiler does not optimize a loop when the
	//body includes complex function calls or the initiation interval is
	//higher than 14 or the dynamic cycle count is more than 48 etc. Any of
	//those conditions could be triggered by including this function call.
	//Using _spint intrinsic results in the use of 4-cycle/1-FU delay/3
	//delay instruction SPINT, which now alleviates all the earlier problem
	//in loop optimization. The result is that the code is heavily
	//parallelized by the compiler, as is obvious from the included (doc
	//folder) disassembly code listings, and the presence of ||'s, which
	//indicate parallel-issued instructions. So it's not just the direct
	//cost of SPINT vs __c6xabi_fixfu, but this indirect effect on compiler
	//optimization which creates the huge cycle count difference.

	//---------------------- done metric computation ----------------------
//#ifdef DEBUG_MODE
//	debug_metricFBuf[i] = currMetricF;
//#endif
	_amem4(&debug_magNumerBuf[i]) = magASqSqTmp;
	_amem4(&debug_magDenomBuf[i]) = magESqSqTmp;

	idxE2 = (idxE2 + 1) % (SYNC_WINDOW_SIZE_ENERGY + SYNC_BUFFER_SIZE_ENERGY);
	_amem4(&debug_metricBuf[i]) = currMetric;
    }

    _amem4(positionNextE2Value) = idxE2;
}


void ORILIB_SchmidlCoxMetricComputer_i(
		IN	ORILIB_t_GainStateAux * gainStateAux,
		IN 	ORILIB_t_CorrState * corrState,
		IN  	ORILIB_t_CorrStateAux * corrStateAux
		){

	Cplx16	* winCorrBuf = corrStateAux->winCorrBuf;

	inner(
	  //-- energy-only related fields --
	  IN	gainStateAux->windowEnergyE1andE2Buf_withGain,			
		  //buffer to hold window energy values, length = SYNC_BUFFER_SIZE_ENERGY

	  INOUT	&corrState->corr_positionNextE2Value,


	  //-- correlation computation specific fields --
	  IN	winCorrBuf,

	  //-- for debugging --
	  //OUT	corrState->debug_metricFBuf,

	  //-- not just for debugging --
	  OUT	corrState->debug_magNumerBuf,

	  //-- not just for debugging --
	  OUT	corrState->debug_magDenomBuf,

	  //-- not just for debugging --
	  INOUT	corrState->debug_metricBuf
	  );
}
