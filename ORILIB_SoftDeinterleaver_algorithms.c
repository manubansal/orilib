/**
Atomix project, ORILIB_SoftDeinterleaver_algorithms.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include <osl/inc/swpform.h>
#include <osl/inc/or_types.h>

void ORILIB_SoftDeinterleaver_lut_implementation (
	IN 	SoftBit const * const restrict interleaved_bits_in, 
	IN	Uint32 num_bits,
	IN	Uint16 const * const restrict lut_pointer,
	OUT 	SoftBit * const restrict deinterleaved_bits_out
	){

	/* Look up the appropriate table and bit vals */
	uint32_t i=0;
	uint64_t temp_bits_in, output_indices_0123, output_indices_4567;
	uint64_t output_indices_x0x2, output_indices_x4x6, output_indices_x1x3, output_indices_x5x7;
	
	
#pragma MUST_ITERATE(48/8,288/8,48/8)
	for (i=0; i < num_bits; i+=8)
	{
		/* Read 8 bytes out of the input bits array - 0, 1, 2, 3, 4, 5, 6, 7, 8 */
		temp_bits_in = _amem8_const((interleaved_bits_in + i));
		
		/* Read 4 half words of indices - 0,1,2,3 */
		output_indices_0123 = _amem8_const((lut_pointer + i));
		/* Read another 4 half words of indices - 4,5,6,7 */
		output_indices_4567 = _amem8_const((lut_pointer + i + 4));
		
		

		/* Use SIMD instructions to move each of the indices to the 
		 * upper and lower words of a long long. Now we just have to grab
		 * the upper and lower halves for each run through the algorithm. */
		output_indices_x0x2 = _dshru(output_indices_0123,16); // shift HI and LO right by 16 bits
		output_indices_x1x3 = output_indices_0123 & ((long long)(0x0000FFFF0000FFFF));
		output_indices_x4x6 = _dshru(output_indices_4567,16);
		output_indices_x5x7 = output_indices_4567 & ((long long)(0x0000FFFF0000FFFF));
		
		/* Write one unpacked bit (one BYTE) at a time to the output array.
		 * Because bytes are spread all over the symbol now, 
		 * this is basically the simplest, but still fairly efficient way to interleave.
		 * Last benchmark - 436 cycles for 64QAM */
		{
		    /* Declare a variable such that we can look at a single unpacked bit of the input */
		    uint8_t const * const restrict temp_bits_in_ptr8 = (uint8_t*)&temp_bits_in;
		    deinterleaved_bits_out[ _hill(output_indices_x0x2) ] =  temp_bits_in_ptr8[0]; //hword 0
		    deinterleaved_bits_out[ _hill(output_indices_x1x3) ] =  temp_bits_in_ptr8[1]; //hword 1
		    deinterleaved_bits_out[ _loll(output_indices_x0x2) ] =  temp_bits_in_ptr8[2]; //hword 2
		    deinterleaved_bits_out[ _loll(output_indices_x1x3) ] =  temp_bits_in_ptr8[3]; //hword 3
		    
		    deinterleaved_bits_out[ _hill(output_indices_x4x6)  ] =  temp_bits_in_ptr8[4]; //hword 4
		    deinterleaved_bits_out[ _hill(output_indices_x5x7)  ] =  temp_bits_in_ptr8[5]; //hword 5
		    deinterleaved_bits_out[ _loll(output_indices_x4x6)  ] =  temp_bits_in_ptr8[6]; //hword 6
		    deinterleaved_bits_out[ _loll(output_indices_x5x7)  ] =  temp_bits_in_ptr8[7]; //hword 7
		}
	}

}
