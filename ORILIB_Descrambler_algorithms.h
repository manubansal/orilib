/**
Atomix project, ORILIB_Descrambler_algorithms.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_DESCRAMBLER_ALGORITHMS_H_
#define ORILIB_DESCRAMBLER_ALGORITHMS_H_

#include "ORILIB_Descrambler_t.h"
#include "ORILIB_t.h"

void ORILIB_Descrambler_i_internal (
	IN  uint64_t 				* const restrict data_in, 
	IN  ORILIB_t_Descrambler_State 		* restrict inpState, 
	OUT uint64_t 				* const restrict data_out,
	OUT ORILIB_t_Descrambler_State 		* restrict outState, 
	IN  uint32_t 				data_length_in_bits,
	IN  uint32_t 				n_double_words
	);

void ORILIB_Descrambler_internal_xor(
  uint64_t * const restrict data_in,
  uint64_t * const restrict data_out,
  uint8_t * const restrict bit_aligned_sequence,
  uint32_t const sequence_byte_index,
  uint32_t const n_double_words
  );
		

void ORILIB_Descrambler_internal_pkt_init(
  uint8_t first_byte,
  ORILIB_t_Descrambler_State * const restrict outState
  );


/** 
 * @brief Descrambles the input using the scrambling sequence stored in the state parameter.
 *
 * Descrambles the input by XOR-ing 64 bits of the input with 64 bits of the scrambling
 * sequence. Using the number of bits scrambled, this function computes which of the
 * 8 shifted scrambling sequences to use in order to start scrambling at the correct
 * bit.
 *
 * @param[in] data_in is the input scrambled bits.
 * @param[in,out] state is a struct that holds bits_scrambled, the data_length and and the scrambling sequences 2D array. 
 * @param[out] data_out is the output descrambled bits*/

#endif

