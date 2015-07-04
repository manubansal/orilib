/**
Atomix project, ORILIB_MapperBPSK_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>

#include "ORILIB_MapperBPSK_t.h"
#include "ORILIB_utils_inl.h"

#define BITS_PER_WIFI_SYMBOL_BPSK 	48

void ORILIB_MapperBPSK_i ( 
	IN 	ORILIB_t_ByteBuf48   		 * inpBitsBuf,
	OUT	ORILIB_t_Cplx16Buf48     * outSampleBuf,
	CF  ORILIB_MapperBPSK_t_Conf * conf
	) {

	Cplx16* table = conf->lut.samples;
	Cplx16* modulated_symbols = outSampleBuf->samples;
	Uint8* input_bits = inpBitsBuf->bytes;

	uint32_t bit_counter;
	uint64_t current_input_bits;
	/* Create an 8-bit pointer into the current bits */
	
	/* Create the pattern to match against */
	const uint64_t bit_pattern_match = 0xFFFFFFFFFFFFFFFF;
	uint32_t one_or_zero_8bits;
	
	/*Process 8 bits at a time - 48 bits total, so 6 iterations */
#pragma MUST_ITERATE(BITS_PER_WIFI_SYMBOL_BPSK/8,BITS_PER_WIFI_SYMBOL_BPSK/8,BITS_PER_WIFI_SYMBOL_BPSK/8)
	for (bit_counter=0; bit_counter < BITS_PER_WIFI_SYMBOL_BPSK; bit_counter+=8)
	{
		/*Load 8 unpacked bits - 0x00 for 0 or 0xFF for 1 */
		current_input_bits = _amem8( (void*)(input_bits + bit_counter) );
		
		/* Do the SIMD matching - here, we compare the input unpacked bits
		 * against an 8-unpacked-bit pattern of all 1's (i.e. 0xFF..FF).
		 * The results are packed into the 8 LSBs of the output  */
		one_or_zero_8bits = _dcmpeq4(current_input_bits,bit_pattern_match);
		
		/* From the matching, the 8 lsbs tell us whether the result is
		 * a zero or a one. Now, we have to extract each complex symbol
		 * from the LUT, and load it into the output complex array.
		 * Each bit is now 32 bits, so we can load two bits out at a
		 * time. This requires 4 _amem8 writes per loop */
		 
		 /* Use extu: csta = 31-MSB, cstb= csta+LSB */
		 /* Output the first two symbols */
		 _amem8( (void*)(modulated_symbols+ bit_counter )) = 
		 		_itoll( _amem4( (void*)(table + _extu(one_or_zero_8bits,24,31)) ),
		 				_amem4( (void*)(table + _extu(one_or_zero_8bits,25,31)) ) );
		 
		 /* Output the second two symbols */
		 _amem8( (void*)(modulated_symbols+ bit_counter + 2)) =
		 		_itoll( _amem4( (void*)(table + _extu(one_or_zero_8bits,26,31)) ),
		 				_amem4( (void*)(table + _extu(one_or_zero_8bits,27,31)) ) );
		 
		 /* Output the third two symbols */
		 _amem8( (void*)(modulated_symbols+ bit_counter + 4)) =
		 		_itoll( _amem4( (void*)(table + _extu(one_or_zero_8bits,28,31)) ),
		 				_amem4( (void*)(table + _extu(one_or_zero_8bits,29,31)) ) );
		 
		 /* Output the last two symbols */
		 _amem8( (void*)(modulated_symbols+ bit_counter + 6 )) = 
				_itoll( _amem4( (void*)(table + _extu(one_or_zero_8bits,30,31)) ),
		 				_amem4( (void*)(table + (one_or_zero_8bits&0x01) ) ) );
	}
}

void ORILIB_MapperBPSK_i_conf(
	CF ORILIB_MapperBPSK_t_Conf * conf,
	Uint32 scaleFactor){
	conf->lut.samples[1].real = scaleFactor;
	conf->lut.samples[1].imag = 0;
	conf->lut.samples[0].real = -scaleFactor;
	conf->lut.samples[0].imag = 0;
}
