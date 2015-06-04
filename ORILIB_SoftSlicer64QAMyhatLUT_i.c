/**
Atomix project, ORILIB_SoftSlicer64QAMyhatLUT_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include "ORILIB_SoftSlicer64QAMyhatLUT_t.h"
#include "WIFILIB_util.h"
#include "WIFILIB_lookupTables.h"

#define NUM_POINTS_PER_CALL 48


//void SoftDemapper_64qamFromLut(
//			IN 	Cplx16	const * const restrict yhatArray,
//			OUT	BitsSoft * const restrict	normalizedSoftEstimates,
//			INOUT SoftDemapperWifiState * const state) {
void ORILIB_SoftSlicer64QAMyhatLUT_i (
			IN  ORILIB_t_Cplx16Buf48 	* inpBufyhat,
			OUT ORILIB_t_SoftBitBuf288	* outBufSoftBits
	){

	typedef union {
		uint64_t dblword;
		Cplx16 dblCplx16[2];
		uint8_t bytes[8];
	} myunion;

	uint32_t i, output_index;
//	uint64_t pntdbl;
	myunion dblword;
//	uint8_t *I1, *Q1, *I2, *Q2;

	uint32_t eI1, eQ1, eI2, eQ2;
	uint32_t eQ1left, eQ2left, eI2left;

//	I1 = ((uint8_t *)&pntdbl) + 1;
//	Q1 = ((uint8_t *)&pntdbl) + 3;
//	I2 = ((uint8_t *)&pntdbl) + 5;
//	Q2 = ((uint8_t *)&pntdbl) + 7;


	{

	Cplx16 const * const restrict yhatArray = inpBufyhat->samples;
	BitsSoft * const restrict normalizedSoftEstimates = (BitsSoft *)outBufSoftBits->softBits;

	//uint32_t * const restrict softSlicingTable64qam = state->lut_64qam;


//	ASSERT_PTR_ALIGNED(yhatArray, 8);
//	ASSERT_PTR_ALIGNED(normalizedSoftEstimates, 4);
//	assert(nPoints % 2 == 0);
#pragma MUST_ITERATE(NUM_POINTS_PER_CALL/2,NUM_POINTS_PER_CALL/2,NUM_POINTS_PER_CALL/2);
	for (i = 0, output_index =0; i < NUM_POINTS_PER_CALL; i+= 2, output_index +=12)
	{
//		pntdbl = _amem8_const(&yhatArray[i]);

//		eI1 = WIFILIB_softSlicingTable64qam[*I1];
//		eQ1 = WIFILIB_softSlicingTable64qam[*Q1];
//		eI2 = WIFILIB_softSlicingTable64qam[*I2];
//		eQ2 = WIFILIB_softSlicingTable64qam[*Q2];

		dblword.dblword = _amem8_const(&yhatArray[i]);

		eI1 = WIFILIB_softSlicingTable64qam[dblword.bytes[1]];
		eQ1 = WIFILIB_softSlicingTable64qam[dblword.bytes[3]];
		eI2 = WIFILIB_softSlicingTable64qam[dblword.bytes[5]];
		eQ2 = WIFILIB_softSlicingTable64qam[dblword.bytes[7]];



		eQ1left = _shlmb(0, eQ1);
		eQ2left = _shlmb(0, eQ2);
		eI2left = _shlmb(0, eI2);

		_amem4(&normalizedSoftEstimates[output_index])     = _shlmb(eQ1left, eI1);
		_amem4(&normalizedSoftEstimates[output_index + 4]) = _packlh2(eQ1, eI2left);
		_amem4(&normalizedSoftEstimates[output_index + 8]) = _shrmb(eI2, eQ2left);

	}

	}
}
