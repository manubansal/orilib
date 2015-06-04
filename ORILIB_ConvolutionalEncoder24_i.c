/**
Atomix project, ORILIB_ConvolutionalEncoder24_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>

#include "ORILIB_t.h"
#include "WIFILIB_t.h"
#include "ORILIB_ConvolutionalEncoder24_t.h"

void ORILIB_ConvolutionalEncoder24_i (
      IN 	ORILIB_t_BitBucket24 * inpBitsBuf,
      IN  ORILIB_ConvolutionalEncoder24_t_State * inpStateBuf,
      OUT ORILIB_t_BitBucket24 * outEncodedBitsA,
      OUT ORILIB_t_BitBucket24 * outEncodedBitsB,
      OUT ORILIB_ConvolutionalEncoder24_t_State * outStateBuf
      ) {

	/* Internal, temporary variables: */
	uint32_t dword_counter = 0;
	uint64_t prev6_and_current63to6,
		prev5_and_current63to5,
		prev3_and_current63to3,
		prev2_and_current63to2,
		prev1_and_current63to1,
		current_dword, tmp_shared;

	Uint64* input_bits = (Uint64*)inpBitsBuf->packedBits;
	Uint64* encoded_bits_a = (Uint64*)outEncodedBitsA->packedBits;
	Uint64* encoded_bits_b = (Uint64*)outEncodedBitsB->packedBits;
	
	/* Local copies of state information */
	uint64_t previousSixInputBits = inpStateBuf->previousSixInputBits;
	uint32_t num_input_bits = kWifiGeeDataRatesInfo[kWifiGee6mbps].num_data_bits_per_ofdm_symbol;
	
	
	/* Determine how many 64 bit elements are going to be processed 
	 * in this iteration. This is dependent upon the number of data bits 
	 * per symbol, and includes partial double words:
	 * 			num			= ceil(numberOfInputBits/64) */
	uint32_t num_dword_containers = 
					(num_input_bits>>6) + ( (num_input_bits&0x3F) ? 1:0);
	
	/* How many bits are in the leftover portion? 
	 * - Create a mask to keep only these bits for the last dword */
	uint32_t least_significant_bit_in_last_dword = ( (num_dword_containers<<6) - num_input_bits);
	uint64_t extra_bit_mask = ((uint64_t)0xFFFFFFFFFFFFFFFF)<< least_significant_bit_in_last_dword;
	
	/* For each of these dword packed bit elements, 
	 * calculate the A and B convolutionally encoded outputs */
	for (dword_counter = 0; dword_counter < num_dword_containers; dword_counter++)
	{
		
		/* Get the current double word's worth of packed bits */
		current_dword = _amem8( (input_bits + dword_counter) );
		
		/* Shift the last 6 bits of the previous word into the 6 MSBs of the current word container: */
		prev6_and_current63to6 = (previousSixInputBits<<(64-6)) | current_dword>>6;
		prev5_and_current63to5 = (previousSixInputBits<<(64-5)) | current_dword>>5;
		prev3_and_current63to3 = (previousSixInputBits<<(64-3)) | current_dword>>3;
		prev2_and_current63to2 = (previousSixInputBits<<(64-2)) | current_dword>>2;
		prev1_and_current63to1 = (previousSixInputBits<<(64-1)) | current_dword>>1;
		
		/* Calculate the portion that is common to the A and B encoders: 0,-2, -3, -6 */
		tmp_shared = current_dword ^  //0
						prev2_and_current63to2 ^ //-2
						prev3_and_current63to3 ^ //-3
						(prev6_and_current63to6); //-6
		
		/* Write the encoded bits out */
		_amem8(encoded_bits_a + dword_counter) = tmp_shared ^ prev5_and_current63to5; //-5
		_amem8(encoded_bits_b + dword_counter) = tmp_shared ^ prev1_and_current63to1; //-1
		
		/* Store the 6 LSBs for the next iteration */
		previousSixInputBits = current_dword&((long long)0x000000000000003F); 
	}
	
	/* Store the previous six input bits back to the state variable 
	 * for the next symbol calculation. Since the last iteration may have included 
	 * a partial double word, the last 6 bits could be somewhere in the middle. 
	 * Deal with this case: */
	outStateBuf->previousSixInputBits = (current_dword>>least_significant_bit_in_last_dword)&
														((long long)0x000000000000003F);
	 
	/* Zero out the unused portions of the 64 bit containers, 
	 * just for sanity's sake */
	encoded_bits_a[dword_counter-1] &= extra_bit_mask;
	encoded_bits_b[dword_counter-1] &= extra_bit_mask;
}
