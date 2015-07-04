/**
Atomix project, ORILIB_SoftDepuncturer_algos.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include "ORILIB_t.h"

//void DepuncturerWifi_DepunctureTwoThirds(IN 	BitsSoft const * const restrict bits_soft,
//											OUT 	BitsSoft  * const restrict depunctured_soft_bits, 
//										INOUT DepuncturerWifiState * const restrict state)
//{
//	uint32_t num_coded_bits = kWifiGeeDataRatesInfo[state->data_rate].num_coded_bits_per_ofdm_symbol;
//	/* Only for 48 mbps */
//	assert(state->data_rate == kWifiGee48mbps);
//	
//	uint32_t input_bit_index, output_bit_index, i;
//	
//	uint64_t bits_0_to_47[6];
//	
//	/** To get the best possible read/write strides, we read in 12 words worth of soft bits 
//	 * (i.e. 48 bits) and write out 16 words of soft bits (i.e. 64 soft bits). */
//	for (input_bit_index =0, output_bit_index=0; 
//			input_bit_index < num_coded_bits; 
//			input_bit_index+=48, output_bit_index+=64)
//	{	
//		/* Read in 12 words of inputs */
//#pragma MUST_ITERATE(6,6,6)
//		for(i=0; i<6; i++)
//		{
//			bits_0_to_47[i] = _amem8_const( ((uint64_t*)(bits_soft + input_bit_index)) + i);
//		}
//		
//		/* write out the first 2 words, i.e. bytes 0-7. pattern here is:
//		 * [0 1 2 3 | 4 5 6 7 ] -> [ 0 1 2 x | 3 4 5 x], where x=0 for soft bit 50/50 */ 
//		
//		_amem8(depunctured_soft_bits + output_bit_index + 0) = _itoll(
//				(_hill(bits_0_to_47[0])&0xFFFFFF00),
//				( _shrmb(_hill(bits_0_to_47[0]),_loll(bits_0_to_47[0]))&0xFFFFFF00) );
//		
//		/* write out the second 2 words, i.e. bytes 8-15. pattern here is:
//		 * [4 5 6 7 | 8 9 10 11] -> [ 6 7 8 x | 9 10 11 x] , where x=0 for soft bit 50/50 */ 
//		_amem8(depunctured_soft_bits + output_bit_index + 8) = _itoll(
//				(_packlh2(_loll(bits_0_to_47[0]),_hill(bits_0_to_47[1]))&0xFFFFFF00),
//				( (_hill(bits_0_to_47[1]))<<8) );
//				
//		/* write out the third 2 words, i.e. bytes 16-23. pattern here is:
//		 * [12 13 14 15 | 16 17 18 19] -> [ 12 13 14 x | 15 16 17 x] , where x=0 for soft bit 50/50 */ 
//		_amem8(depunctured_soft_bits + output_bit_index + 16) = _itoll(
//				(_loll(bits_0_to_47[1])&0xFFFFFF00),
//				( _shrmb(_loll(bits_0_to_47[1]),_hill(bits_0_to_47[2]))&0xFFFFFF00) );
//				
//		/* write out the fourth 2 words, i.e. bytes 24-31. pattern here is:
//		 * [16 17 18 19 | 20 21 22 23] -> [ 18 19 20 x | 21 22 23 x ] , where x=0 for soft bit 50/50 */ 
//		_amem8(depunctured_soft_bits + output_bit_index + 24) = _itoll(
//				(_packlh2(_hill(bits_0_to_47[2]),_loll(bits_0_to_47[2]))&0xFFFFFF00),
//				( ((_loll(bits_0_to_47[2]))<<8) ) );
//				
//		/* write out the fifth 2 words, i.e. bytes 32-39. pattern here is:
//		 * [24 25 26 27 | 28 29 30 31 ] -> [ 24 25 26 x | 27 28 29 x ] , where x=0 for soft bit 50/50 */ 
//		_amem8(depunctured_soft_bits + output_bit_index + 32) = _itoll(
//				(_hill(bits_0_to_47[3])&0xFFFFFF00),
//				( _shrmb(_hill(bits_0_to_47[3]),_loll(bits_0_to_47[3]))&0xFFFFFF00) );
//		
//		/* write out the sixth 2 words, i.e. bytes 8-15. pattern here is:
//		 * [28 29 30 31 |32 33 34 35] -> [30 31 32 x | 33 34 35 x] , where x=0 for soft bit 50/50 */ 
//		_amem8(depunctured_soft_bits + output_bit_index + 40) = _itoll(
//				(_packlh2(_loll(bits_0_to_47[3]),_hill(bits_0_to_47[4]))&0xFFFFFF00),
//				( (_hill(bits_0_to_47[4]))<<8) );
//				
//		/* write out the seventh 2 words, i.e. bytes 16-23. pattern here is:
//		 * [12 13 14 15 | 16 17 18 19] -> [ 12 13 14 x | 15 16 17 x] , where x=0 for soft bit 50/50 */ 
//		_amem8(depunctured_soft_bits + output_bit_index + 48) = _itoll(
//				(_loll(bits_0_to_47[4])&0xFFFFFF00),
//				( _shrmb(_loll(bits_0_to_47[4]),_hill(bits_0_to_47[5]))&0xFFFFFF00) );
//				
//		/* write out the eighth 2 words, i.e. bytes 24-31. pattern here is:
//		 * [16 17 18 19 | 20 21 22 23] -> [ 18 19 20 x | 21 22 23 x ] , where x=0 for soft bit 50/50 */ 
//		_amem8(depunctured_soft_bits + output_bit_index + 56) = _itoll(
//				(_packlh2(_hill(bits_0_to_47[5]),_loll(bits_0_to_47[5]))&0xFFFFFF00),
//				( ((_loll(bits_0_to_47[5]))<<8) ) );
//	}
//
//
//}
										
void ORILIB_SoftDepuncturerThreeFourths(	
		IN 	BitsSoft const * const restrict bits_soft,
		OUT 	BitsSoft  * const restrict depunctured_soft_bits, 
		IN	Uint32 num_coded_bits	//before depuncturing
		){
//		INOUT DepuncturerWifiState * const restrict state
//		) {
	//uint32_t num_coded_bits = kWifiGeeDataRatesInfo[state->data_rate].num_coded_bits_per_ofdm_symbol;
	//ex. 288 for 64qam

	uint32_t input_bit_index, output_bit_index;
	
	uint64_t bits_0_to_7, bits_8_to_15;
	
	/** To get the best possible read/write strides, we read in 4 words worth of soft bits (i.e. 16 bits)
	 *  and write out 6 words of soft bits (i.e. 24 soft bits). */
	for (input_bit_index =0, output_bit_index=0; input_bit_index < num_coded_bits; input_bit_index+=16, output_bit_index+=24)
	{	
		bits_0_to_7 = _amem8_const(bits_soft + input_bit_index);
		bits_8_to_15 = _amem8_const(bits_soft + input_bit_index + 8);
		
		/* write out the first 2 words, i.e. bytes 0-7. pattern here is:
		 * 0 1 2 x | x 3 4 5, where x=0 for soft bit 50/50 */ 
		
		_amem8(depunctured_soft_bits + output_bit_index + 0) = _itoll(
				(_hill(bits_0_to_7)&0xFFFFFF00),
				(_packlh2(_hill(bits_0_to_7),_loll(bits_0_to_7))&0x00FFFFFF) );
		
		/* Write out the second 2 words, i.e. bytes 8-15. pattern here is: 
		 * 6 x x 7 | 8 9 10 x */
		_amem8(depunctured_soft_bits + output_bit_index + 8) = _itoll(
				(_pack2(_loll(bits_0_to_7),_loll(bits_0_to_7))&0xFF0000FF),
				(_hill(bits_8_to_15)&0xFFFFFF00) );
				
		/* Write out the third 2 words, i.e. bytes 16-23. pattern here is: 
		 * x 11 12 13 | 14 x x 15 */
		 _amem8(depunctured_soft_bits + output_bit_index + 16) = _itoll(
		 		(_packlh2(_hill(bits_8_to_15),_loll(bits_8_to_15))&0x00FFFFFF),
		 		(_pack2(_loll(bits_8_to_15),_loll(bits_8_to_15))&0xFF0000FF) );		
	}
}

