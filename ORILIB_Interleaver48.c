/**
Atomix project, ORILIB_Interleaver48.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>

#include <src/DSP_ifft16x16/c66/DSP_ifft16x16.h>

#include "ORILIB_t.h"

/** Table defines the interleaver bit mapping from input bits to output bits
 * for a BPSK-encoded OFDM symbol */
static const uint16_t InterleaverLutWifi_BpskLUT[48] = { 0, 3, 6, 9, 12, 15, 18, 21, 
													24, 27, 30, 33, 36, 39, 42, 45, 
													1, 4, 7, 10, 13, 16, 19, 22, 
													25, 28, 31, 34, 37, 40, 43, 46, 
													2, 5, 8, 11, 14, 17, 20, 23, 
													26, 29, 32, 35, 38, 41, 44, 47 };

void ORILIB_Interleaver48(
			IN	ORILIB_t_ByteBuf48	* inpSampleBuf,
			OUT	ORILIB_t_ByteBuf48	* outSampleBuf
			) {
			
	uint16_t const * const lut_pointer = InterleaverLutWifi_BpskLUT;
	const uint32_t num_bits = 48;
	
	uint32_t i=0;
	uint64_t temp_bits_in, output_indices_0123, output_indices_4567;
	uint64_t output_indices_x0x2, output_indices_x4x6, output_indices_x1x3, output_indices_x5x7;
	
	/* Declare a variable such that we can look at a single unpacked bit of the input */
	uint8_t const * const temp_bits_in_ptr8 = (uint8_t*)&temp_bits_in;
	
	BitsUnpacked const * const coded_bits_in = inpSampleBuf->bytes;
	BitsUnpacked * const restrict interleaved_bits_out = outSampleBuf->bytes;
	
	#pragma MUST_ITERATE(48/8,288/8,48/8)
	for (i=0; i < num_bits; i+=8)
	{
		/* Read 8 bytes out of the input bits array - 0, 1, 2, 3, 4, 5, 6, 7, 8 */
		temp_bits_in = _amem8((void*)(coded_bits_in + i));
		
		/* Read 4 half words of indices - 0,1,2,3 */
		output_indices_0123 = _amem8((void*)(lut_pointer + i));
		/* Read another 4 half words of indices - 4,5,6,7 */
		output_indices_4567 = _amem8((void*)(lut_pointer + i + 4));
		
		

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
		interleaved_bits_out[ _hill(output_indices_x0x2) ] =  temp_bits_in_ptr8[0]; //hword 0
		interleaved_bits_out[ _hill(output_indices_x1x3) ] =  temp_bits_in_ptr8[1]; //hword 1
		interleaved_bits_out[ _loll(output_indices_x0x2) ] =  temp_bits_in_ptr8[2]; //hword 2
		interleaved_bits_out[ _loll(output_indices_x1x3) ] =  temp_bits_in_ptr8[3]; //hword 3
		
		interleaved_bits_out[ _hill(output_indices_x4x6)  ] =  temp_bits_in_ptr8[4]; //hword 4
		interleaved_bits_out[ _hill(output_indices_x5x7)  ] =  temp_bits_in_ptr8[5]; //hword 5
		interleaved_bits_out[ _loll(output_indices_x4x6)  ] =  temp_bits_in_ptr8[6]; //hword 6
		interleaved_bits_out[ _loll(output_indices_x5x7)  ] =  temp_bits_in_ptr8[7]; //hword 7
	}
			
}
