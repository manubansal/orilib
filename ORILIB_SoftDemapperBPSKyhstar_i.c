/**
Atomix project, ORILIB_SoftDemapperBPSKyhstar_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_SoftDemapperBPSKyhstar_t.h"
#include "WIFILIB_util.h"

#define NUM_POINTS_PER_CALL 48
//#define PHY_SOFT_BITS_PRECISION 					6
#define PHY_SOFT_BITS_PRECISION 					5

//void SoftDemapper_BpskFromYhstar(
//			IN 	Cplx32 const * const restrict yhstarArray,
//			IN	const uint8_t	input_range,
//			IN	const uint8_t 	input_scale,
//			IN	const uint8_t	output_range,
//			OUT	BitsSoft * const restrict normalizedSoftEstimates
//			) {
void ORILIB_SoftDemapperBPSKyhstar_i (
		IN  ORILIB_t_Cplx32Buf48		* inpBufyhstar,
		IN  ORILIB_t_OFDMEqualizer_State	* inpBufEqState,
		OUT ORILIB_t_SoftBitBuf48		* outBufSoftBits
	){

	Cplx32 * yhstarArray = inpBufyhstar->samples;
	Uint8 input_range = inpBufEqState->yhstar_norm_min;
	Uint8 input_scale = 0;
	Uint8 output_range = PHY_SOFT_BITS_PRECISION;
	BitsSoft * normalizedSoftEstimates = outBufSoftBits->softBits;

	uint64_t yhstar_local;
	BitsSoft normalized_estimates_local[8];
	uint32_t i, j;
	
#pragma MUST_ITERATE(NUM_POINTS_PER_CALL/8,NUM_POINTS_PER_CALL/8,NUM_POINTS_PER_CALL/8);
	for (i = 0; i < NUM_POINTS_PER_CALL; i+=8) 
	{
		/** Load 8 points, since we can write out 8, 8-bit points aligned */
#pragma MUST_ITERATE(8,8,8)
		for (j=0; j < 8; j++)
		{
			yhstar_local = _amem8_const( yhstarArray + i + j);
//			saturate(
//				_hill(yhstar_local), // take the real part for BPSK
//				input_scale,
//				input_range,
//				output_range,
//				normalized_estimates_local + j
//				);

			WIFILIB_util_saturate_given_norm(
					_hill(yhstar_local), // take the real part for BPSK
					input_range,
					output_range,
					normalized_estimates_local + j
			);
		}
		_amem8(normalizedSoftEstimates + i) = _amem8(normalized_estimates_local);
	}	
	

	DEBUG_DATA(
	LOG_PRINTF("soft bits after bpsk slicing\n");
	printOctetsDecSigned(outBufSoftBits->softBits, 48);
	)
}

