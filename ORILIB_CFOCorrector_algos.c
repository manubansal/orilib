/**
Atomix project, ORILIB_CFOCorrector_algos.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include "ORILIB_CFOCorrectorG_t.h"
#include "WIFILIB_util.h"

#include "ORILIB_CFOCorrectorGM_t.h"

#include "ORILIB_utils_inl.h"
#include "WIFILIB_lookupTables.h"
//#include "IQmath_inline_all.h"
#include "IQmath_inline.h"


void ORILIB_CFOCorrectorG_inner(
		IN	Cplx16			const * const restrict inSampleBuf,
		IN	ORILIB_t_CFOState 	const * const restrict cfoState,
		IN	ORILIB_t_PktAgcState 	const * const restrict agcState, 
		OUT	Cplx16			* const restrict outSampleBuf
		){
	
	Cplx16U currSample,currSample2, currSample_g,currSample_g2, derot, derot2,cleanedSample;
	Uint64 twosamples, two_clean_samples;
	Uint32 i;
	Uint32 derotIdx = 0;
	Uint32 derotTableStride;
	Cplx16U *derotTable;
	Uint32 gain;


	gain 			= _amem4(&agcState->agcGain);
	derotTableStride 	= cfoState->derot_table_stride;
	derotTable 		= cfoState->derotTablePtr;

	DEBUG_VERBOSE(
	LOG_PRINTF("cfo corrector input agc gain: %d\n", agcState->agcGain);
	)
	
#ifdef DEBUG_MODE
	ASSERT_PTR_ALIGNED(inSampleBuf, 8);
	ASSERT_PTR_ALIGNED(outSampleBuf, 8);
	ASSERT_PTR_ALIGNED(derotTable, 8);
#endif

	//hints to the compiler; don't add any overhead in runtime;
	//may allow the compiler to optimize further
	_nassert((Uint32)inSampleBuf % 8 == 0);
	_nassert((Uint32)outSampleBuf % 8 == 0);
	_nassert((Uint32)derotTable % 8 == 0);

#pragma MUST_ITERATE(SYNC_CFO_CORRECTION_BUF_LEN/2, SYNC_CFO_CORRECTION_BUF_LEN/2, SYNC_CFO_CORRECTION_BUF_LEN/2);
	for (i = 0; i < SYNC_CFO_CORRECTION_BUF_LEN; i+=2) {
		twosamples = _amem8_const(&inSampleBuf[i]);
		currSample.realimag = _hill(twosamples);
		currSample2.realimag = _loll(twosamples);
		
		derot.realimag = _amem4_const(&derotTable[derotIdx]);	//Q14
		derotIdx += derotTableStride;
		derot2.realimag = _amem4_const(&derotTable[derotIdx]);
		derotIdx += derotTableStride;
		//this gets inlined, so doesn't hinder loop optimization.
		//the gain is applied so that keeping only the high bits
		//after complex multiplication doesn't lead to loss of
		//significant bits.
		WIFILIB_util_applyGain(
				&currSample,
				&currSample_g,
				gain
				);
		WIFILIB_util_applyGain(
				&currSample2,
				&currSample_g2,
				gain
				);

		two_clean_samples = 
			  _dcmpyr1(_itoll(currSample_g.realimag,currSample_g2.realimag),
				   _itoll(derot.realimag,derot2.realimag));
		_amem8(&outSampleBuf[i]) = two_clean_samples;
		
	}

	//DEBUG(
	//LOG_PRINTF("symbol after cfo correction and applying gain\n");
	//printCplx16SingleColumn(outSampleBuf, 64);
	//)
}


void ORILIB_CFOCorrectorGM_inner(
		IN	Cplx16			const * const restrict inSampleBuf,
		IN	ORILIB_t_CFOState 	const * const restrict cfoState,
		IN	ORILIB_t_PktAgcState 	const * const restrict agcState, 
		OUT	Cplx16			* const restrict outSampleBuf
		){
	
	Cplx16U currSample, currSample_g, derot, cleanedSample, cleanedSamplef;
	Cplx32U lo_factor_32;
	Cplx16U lo_factor;

	Uint32 factor_Q_value;
	Uint32 derotTableStride;
	Cplx16U *derotTable;
	Uint32 gain;

	Uint32 i;
	Uint32 derotIdx = 0;
	Uint32 shiftl;


	gain 			= _amem4(&agcState->agcGain);
	derotTableStride 	= cfoState->derot_table_stride;
	derotTable 		= cfoState->derotTablePtr;


	//HACK begin
	//cfoEstimatorState->factor_with_norm_atmost_1.realimag = 
	//	derotTable[80 * derotTableStride].realimag;
	//cfoEstimatorState->factor_Q_value = DEROT_TABLE_Q_VALUE;
	//factor_Q_value = cfoEstimatorState->factor_Q_value;
	factor_Q_value = DEROT_TABLE_Q_VALUE;
	lo_factor.realimag = derotTable[80 * derotTableStride].realimag;
	//HACK end



	//convert the factor to Q15 with saturation, so that norm doesn't
	//change upon multiplying with the factor
	shiftl = 31 - factor_Q_value;
	lo_factor_32.cplx32.real = lo_factor.cplx16.real;
	lo_factor_32.cplx32.imag = lo_factor.cplx16.imag;
	lo_factor_32.cplx32.real = _sshvl(lo_factor_32.cplx32.real, shiftl);
	lo_factor_32.cplx32.imag = _sshvl(lo_factor_32.cplx32.imag, shiftl);
	lo_factor.realimag = _packh2(lo_factor_32.cplx32.real, lo_factor_32.cplx32.imag);


#ifdef DEBUG_MODE
	assert(factor_Q_value <= 15);
	ASSERT_PTR_ALIGNED(inSampleBuf, 8);
	ASSERT_PTR_ALIGNED(outSampleBuf, 8);
	ASSERT_PTR_ALIGNED(derotTable, 8);
	assert((SYNC_CFO_CORRECTION_BUF_LEN - 1) * derotTableStride < DEROT_TABLE_SIZE);
#endif

	//hints to the compiler; don't add any overhead in runtime;
	//may allow the compiler to optimize further
	_nassert((Uint32)inSampleBuf % 8 == 0);
	_nassert((Uint32)outSampleBuf % 8 == 0);
	_nassert((Uint32)derotTable % 8 == 0);

#pragma MUST_ITERATE(SYNC_CFO_CORRECTION_BUF_LEN, SYNC_CFO_CORRECTION_BUF_LEN, SYNC_CFO_CORRECTION_BUF_LEN);
	for (i = 0; i < SYNC_CFO_CORRECTION_BUF_LEN; i++) {
		currSample.realimag = _amem4_const(&inSampleBuf[i]);
		derot.realimag = _amem4_const(&derotTable[derotIdx]);	//Q14

		//this gets inlined, so doesn't hinder loop optimization.
		//the gain is applied so that keeping only the high bits
		//after complex multiplication doesn't lead to loss of
		//significant bits.
		WIFILIB_util_applyGain(
				&currSample,
				&currSample_g,
				gain
				);


		//only higher 16 bits of the result are kept
		cleanedSample.realimag = _cmpyr1(currSample_g.realimag, derot.realimag);

		//only higher 16 bits of the result are kept
		cleanedSamplef.realimag = _cmpyr1(cleanedSample.realimag, lo_factor.realimag);

		_amem4(&outSampleBuf[i]) = cleanedSamplef.realimag;

		derotIdx += derotTableStride;
	}
}

