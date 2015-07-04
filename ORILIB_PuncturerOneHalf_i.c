/**
Atomix project, ORILIB_PuncturerOneHalf_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include "ORILIB_t.h"
#include "WIFILIB_t.h"
#include "ORILIB_utils_inl.h"
#include "ORILIB_PuncturerOneHalf_t.h"

#define MAX_PUNCTURED_BITSPACKED_ELEMENTS_PER_CALL 5 // 288 at highest modulation
#define MAX_DATA_BITS_PER_CALL 216 // num data bits in each encoded input stream		

void ORILIB_PuncturerOneHalf_i (
      IN 	ORILIB_t_BitBucket24 * inpBitsBufA,
      IN 	ORILIB_t_BitBucket24 * inpBitsBufB,
      OUT ORILIB_t_ByteBuf48 * outByteBuf
      ) {
	/* Get state info */
	uint32_t num_input_bits = kWifiGeeDataRatesInfo[kWifiGee6mbps].num_data_bits_per_ofdm_symbol; 
	
	/* Declare & initialize local variables */
	Uint64* encoded_bits_a = (Uint64*)inpBitsBufA->packedBits;
	Uint64* encoded_bits_b = (Uint64*)inpBitsBufB->packedBits;	
	Uint8* punctured_bits_unpacked = outByteBuf->bytes;
	
	/* Local copies from the input streams: */
	uint64_t encoded_bits_a_i; 
	uint64_t encoded_bits_b_i;
	/* Use these to index 32 bits at a time: */
	uint32_t * const encoded_bits_a_i_ptr = (uint32_t*)(&encoded_bits_a_i);
	uint32_t * const encoded_bits_b_i_ptr = (uint32_t*)(&encoded_bits_b_i);
	
	/* Determine how many 64 bit elements are going to be processed 
	 * in this function for both the input and output. 
	 * This is dependent upon the number of data bits 
	 * per symbol, and includes partial double words:
	 * 			num			= ceil(numberOfInputBits/64) */
	uint32_t num_dword_containers = 
					(num_input_bits>>6) + ( (num_input_bits&0x3F) ? 1:0);
	/* 			num			= ceil(numberOfInputBits*2/64) */
	
	//uint32_t num_dword_containers_out  = (num_input_bits>>5) + ( (num_input_bits&0x1F) ? 1:0);
	/* Make sure we aren't puncturing too many bits and overloading the algorithm */
	assert( ((num_input_bits>>5) + ( (num_input_bits&0x1F) ? 1:0)) <= MAX_PUNCTURED_BITSPACKED_ELEMENTS_PER_CALL );
	
	BitsPacked_ll punctured_bits[MAX_PUNCTURED_BITSPACKED_ELEMENTS_PER_CALL];
	
	/* Twice as many output bits as input bits */
	uint32_t num_output_bits = (num_input_bits<<1);
	

	uint32_t 	dword_counter,
				thirtytwo_bit_offset, 
				output_counter;
	/* temporary ab_hi and ab_lo variable */
	uint64_t ab_hi_ab_lo=0;


	/* declare a local output array for packed bits */
	
	 /* for each packed bits element */
	for ( dword_counter = 0, output_counter = 0; 
			dword_counter < num_dword_containers; 
			dword_counter++, output_counter+=2 )
	{
		/* load 8 bytes worth of data from each array */
		encoded_bits_a_i = _amem8( (void*)(encoded_bits_a + dword_counter));
		encoded_bits_b_i = _amem8( (void*)(encoded_bits_b + dword_counter));
		
		/* Now start shuffling bits from the streams, 16 bits at a time */
		#pragma MUST_ITERATE(2,2,2);
		for ( thirtytwo_bit_offset = 0; thirtytwo_bit_offset<2; thirtytwo_bit_offset++)
		{
			/* Generate [A0...A15 B0...B15 A16...A31 B16...B31] */
			ab_hi_ab_lo = 
				_dpack2(encoded_bits_a_i_ptr[thirtytwo_bit_offset],
						encoded_bits_b_i_ptr[thirtytwo_bit_offset]);
			
			/* Shuffle [A0...A15 B0...B15 | and | A16...A31 B16...B31] to get:
			 *  [A0 B0 A1 B1 .... A15 B15 | and | A16 B16 ... A31 B31] */
			/* write the double word out to the punctured_bits output */
			_amem8(punctured_bits + output_counter + thirtytwo_bit_offset) = 
					_itoll(_shfl(_hill(ab_hi_ab_lo)), _shfl(_loll(ab_hi_ab_lo)) );
		}
	}
	
	ORLIB_unpack_bits(punctured_bits,num_output_bits,punctured_bits_unpacked);
}
