/**
Atomix project, ORILIB_utils_inl.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

/* ============================================================================
 * Copyright (c) 2010 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * ========================================================================= */

/* Doxygen Documentation: */
/** 	@file orlib_utils_inl.h
 *	@brief This header file declares all inline utility methods for the ORLIB library
 * 	
 * 	@author Jeffrey Mehlman
 * 	@author Manu Bansal
 *
 * 	$Id$
 */

#ifndef ORILIB_UTILS_INL_H_
#define ORILIB_UTILS_INL_H_

/* Includes */
#include <osl/inc/or_types.h>
#include <string.h>
/** This inline method unpacks bits to bytes */
inline void ORLIB_unpack_bits( 	IN BitsPacked_ll const * const restrict input_bits,
								IN const uint32_t num_bits_to_unpack,
		  						OUT BitsUnpacked * const restrict output_bits_unpacked)
{
	
	uint64_t input_bits_i, temp_output;
	/* Create 8 bit pointers into these guys */
	uint8_t const * const temp_output_ptr8 = (uint8_t*)(&temp_output);
	uint8_t const * const input_bits_ptr8 = (uint8_t *)(&input_bits_i); 
	uint32_t input_counter, output_counter=0,i,j, bit_counter;
	
	/* Calculate the full number of 64 bit elements to unpack, 
	 * 		i.e. floor( bits/64) */
	const uint32_t num_64bit_elements_to_unpack = num_bits_to_unpack>>6;
	
	/* Calculate the full number of leftover bytes, 
	 * 		i.e. floor(num_leftover_bits/8) */
	const uint32_t num_leftover_bytes_to_unpack = (num_bits_to_unpack - (num_64bit_elements_to_unpack<<6))>>3;
	
	/* Calculate the number of leftover bits */
	const uint32_t num_leftover_bits_to_unpack = num_bits_to_unpack - (num_64bit_elements_to_unpack<<6) - (num_leftover_bytes_to_unpack<<3); 
	
	/* Unpack the 64 bit elements */
	for(input_counter=0; input_counter < num_64bit_elements_to_unpack; input_counter++, output_counter+=64)
	{
		/* Load input i from memory */
		input_bits_i = _amem8((void*)(input_bits + input_counter));
		
		/* for each set of 8 bits, unpack to (8 bytes) element - 64 bytes per loop */
#pragma MUST_ITERATE(8,8,8);
		for(i=0; i< 8; i++)
		{
			/* Unpack 8 bits to 8 bytes */
			temp_output = _dxpnd4( input_bits_ptr8[i] );
			/* Store to memory of output, a ui8 type. Increment by i*8 */
			_amem8( output_bits_unpacked + (i<<3) + output_counter ) = temp_output;
		}
	}
	
	/* Get the last 64 bit element + garbage from the input packed bits */
	input_bits_i = _amem8((void*)(input_bits + num_64bit_elements_to_unpack));
	
	/* Unpack the bytes */
	for(j=0; j < num_leftover_bytes_to_unpack; j++)
	{
		/* Unpack 8 bits to 8 bytes */
		temp_output = _dxpnd4( input_bits_ptr8[j] );
		_amem8( output_bits_unpacked + (j<<3) + output_counter ) = temp_output; /* write to the real output */
	}
	
	/* Unpack the bits: */
	/* Grab the final byte and unpack it to the temp output */
	temp_output = _dxpnd4( input_bits_ptr8[num_leftover_bytes_to_unpack] );
	
	for(bit_counter=0; bit_counter < num_leftover_bits_to_unpack; bit_counter++)
	{
		output_bits_unpacked[output_counter + (num_leftover_bytes_to_unpack<<3) + bit_counter] = temp_output_ptr8[bit_counter];
	}
}		


inline void ORLIB_pack_bits( 	IN BitsUnpacked const * const restrict input_unpacked_bits,
								OUT BitsPacked_ll * const restrict output_packed_bits,
								IN const uint32_t num_bits_to_pack,
								IN const uint8_t one_mapping // 0xFF or 0x01
		  						)
{
	uint64_t test_for_8_ones;
	//uint64_t temp_last;
	uint8_t temp_output[8];
	
	
	uint32_t input_counter, output_counter,i, temp;
	
	uint8_t * const restrict idx_to_temp_ptr8 = (uint8_t *)&temp;
	
	/* Copy the one mapping to 64 bit element */
	memset(&test_for_8_ones,one_mapping,8);
	
	for(input_counter=0, output_counter=0;
			 input_counter < num_bits_to_pack; 
			 input_counter+=64, output_counter++)
	{
#pragma MUST_ITERATE(8,8,8);
		for(i=0; i<8; i++)
		{
			temp = _dcmpeq4(test_for_8_ones,_mem8_const(input_unpacked_bits + input_counter + (i<<3) ));
			temp_output[i] = idx_to_temp_ptr8[3];
		}
		_amem8(output_packed_bits + output_counter) = _amem8(temp_output);
	}
//	//reload the last one and zero out unused bits
//	uint32_t leftover_bits_to_mask_off = input_counter - num_bits_to_pack;
//	uint32_t hi_bits = (leftover_bits_to_mask_off>>5) - 1;
//	uint32_t lo_bits = leftover_bits_to_mask_off - 1; //-1 to 31
//	
//	lo_bits = (lo_bits < 32 ? lo_bits : 0); //saturate
//	hi_bits = (hi_bits < 32 ? hi_bits : 0); //saturate
//	const uint32_t mask=0xFFFFFFFF;
//	
//	temp_last = _amem8(output_packed_bits + output_counter -1);
//	_amem8(output_packed_bits + output_counter-1) =
//						_itoll(
//							_hill(temp_last)&_clr(mask,0,(hi_bits)),
//							_loll(temp_last)&_clr(mask,0,(lo_bits)));
	
}


#endif /* ORILIB_UTILS_INL_H_ */

/* ============================================================================
*            Copyright (c) 2011 Stanford University.          
*                           All Rights Reserved.                          
* ========================================================================== */
