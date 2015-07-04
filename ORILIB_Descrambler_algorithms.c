/**
Atomix project, ORILIB_Descrambler_algorithms.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_Descrambler_t.h"
#include "ORILIB_t.h"
#include "ORILIB_Descrambler_algorithms.h"
#include <osl/inc/amem8cpy_inl.h>

void ORILIB_Descrambler_i_internal (
	IN  uint64_t 				* const restrict data_in, 
	IN  ORILIB_t_Descrambler_State 		* restrict inpState, 
	OUT uint64_t 				* const restrict data_out,
	OUT ORILIB_t_Descrambler_State 		* restrict outState, 
	IN  uint32_t 				data_length_in_bits,
	IN  uint32_t 				n_double_words
	){
							  	
	//UNPACK INPUT STATE

	// Extract bits scrambled so far, and the scrambling sequence 2d array
	uint32_t n_bits_scrambled = inpState->n_bits_scrambled;
	uint8_t (*bit_aligned_scrambling_sequences)[BIT_ALIGNED_SEQUENCE_LENGTH_IN_BYTES] = inpState->extendedDescramblerSequence;
	
	// Determine which sequence, and in which byte to start scrambling within the scrambling sequence 2d array
	uint32_t next_bit_index = n_bits_scrambled % PRIMARY_SCRAMBLER_SEQUENCE_LENGTH_IN_BITS; 
	uint32_t bit_aligned_sequence_index = next_bit_index & 0x7; // equivalent to %8
	uint32_t sequence_byte_index = next_bit_index>>3 ;

	uint8_t* bit_aligned_sequence = bit_aligned_scrambling_sequences[bit_aligned_sequence_index];
	
	
	ORILIB_Descrambler_internal_xor(
	  data_in,
	  data_out,
	  bit_aligned_sequence,
	  sequence_byte_index,
	  n_double_words
	  );
	
	//PACK OUTPUT STATE
	// Increment bits scrambled by the number of bits processed.
	outState->n_bits_scrambled = inpState->n_bits_scrambled + data_length_in_bits;

	//copy extendedDescramblerSequence
	_amem8cpy(
	  outState->extendedDescramblerSequence,
	  inpState->extendedDescramblerSequence,
	  48);


//	DEBUG(
//	static int count = 0;
//	LOG_PRINTF("descrambler count : %3d, output:    \n", count++);
////	LOG_PRINTF("descrambler input : ");
////	TEST_printDebug_descrambledBits((Uint8 *)data_in, data_length_in_bits/8);
////	LOG_PRINTF("descrambler output: ");
//	TEST_printDebug_descrambledBits((Uint8 *)data_out, data_length_in_bits/8);
//	)
}



// For each 64 bit frame generated, take 64 bits of input and xor it with 64 bits from the 
// corresponding scrambler sequence. Write output frame to data_out
void ORILIB_Descrambler_internal_xor(
  uint64_t * const restrict data_in,
  uint64_t * const restrict data_out,
//  uint8_t * const restrict bit_aligned_sequence,
  uint8_t * const bit_aligned_sequence,
  uint32_t const sequence_byte_index,
  uint32_t const n_double_words
  ){
		
  int i;
  uint64_t output_frame;
  uint64_t sequence_frame;
  uint64_t input_frame;

//  for (i = 0; i < n_double_words; i++) {
//	  input_frame = ( data_in)[i];
//	  sequence_frame = _mem8( bit_aligned_sequence + sequence_byte_index + i*8);
//
//	  output_frame = sequence_frame ^ input_frame;
//	  _amem8(&(data_out)[i]) = output_frame;
//  }

  ASSERT_PTR_ALIGNED(data_in, 8);
  {
  uint8_t const * restrict seq_ptr = (uint8_t *)(bit_aligned_sequence + sequence_byte_index);
  for (i = 0; i < n_double_words; i++) {
	  input_frame = _amem8(&data_in[i]);
	  sequence_frame = _mem8(seq_ptr);
	  seq_ptr += 8;
	  
	  output_frame = sequence_frame ^ input_frame;
	  _amem8(&(data_out)[i]) = output_frame; 
  }
  }

}



void ORILIB_Descrambler_internal_pkt_init(
  uint8_t first_byte,
  ORILIB_t_Descrambler_State * const restrict outState
  ) {
    uint8_t seedState = first_byte;
    seedState >>= 1; // Only want the first 7 bits since the first 7 input bits are guaranteed to be zeros
    
    uint8_t current_state = seedState;
    
    uint32_t i,j;
    uint8_t shifted_state = 0, output_frame = 0;
    uint8_t (*bit_aligned_scrambling_sequences)[BIT_ALIGNED_SEQUENCE_LENGTH_IN_BYTES] = 
    	outState->extendedDescramblerSequence;

    
    current_state <<= 1; 		
    current_state |= ((current_state ^ (current_state << 3)) >> 7) & 0x01;
    bit_aligned_scrambling_sequences[0][0] = current_state; 
    
    current_state &= 0x7F;				// 0 x7 x6 x5  x4 x3 x2 x1
    current_state <<= 1;				//x7 x6 x5 x4  x3 x2 x1 0
								    
    uint8_t temp_sequence;
    // This for loop generates the primary scrambler sequence of 127 bits a byte at a time, 
    // and repeats it for a total length of 48 bytes. 48 bytes is enough so that a data symbol 
    // of 216 bit would not have to wrap around at any point in the sequence.
    for (i = 1; i < BIT_ALIGNED_SEQUENCE_LENGTH_IN_BYTES; i++) {
	    temp_sequence = 0;
	    
	    shifted_state = current_state << 3;						   			 	
	    //x4 x3 x2 x1  0  0  0  0

	    output_frame = current_state ^ shifted_state;	   						
	    //s0 s1 s2 s3  j4 j5 j6 j7
	    output_frame >>= 4;							      						
	    //0  0  0  0   s0 s1 s2 s3       
	    temp_sequence |= output_frame;											
	    //0  0  0  0   s0 s1 s2 s3
	    current_state <<= 4;						       						
	    //x3 x2 x1 0   0  0  0  0
	    current_state |= (output_frame << 1);			   						
	    //x3 x2 x1 s0  s1 s2 s3 0
																		    //y7 y6 y5 y4  y3 y2 y1 0

	    shifted_state = current_state;					
	    shifted_state <<= 3;						       						
	    //y4 y3 y2 y1  0  0  0  0
	    
							    
	    temp_sequence <<= 4;													
	    //create space for 4 more bits
	    output_frame = current_state ^ shifted_state;	  		 				
	    //t0 t1 t2 t3  j4 j5 j6 j7
	    output_frame >>= 4;								   						
	    //0  0  0  0   t0 t1 t2 t3
							    
	    bit_aligned_scrambling_sequences[0][i] = temp_sequence | output_frame;	
	    //s0 s1 s2 s3  t0 t1 t2 t3
	    current_state <<= 4;							   						
	    //y3 y2 y1 0   0  0  0  0
	    current_state |= (output_frame << 1);			   						
	    //y3 y2 y1 t0  t1 t2 t3 0
	    
    }
    
    // Next, we take the 48 byte scrambling sequence, and create 7 more copies, each left shifted 
    // by 1 more bit. Each of these sequences are generated with 64 bit aligned operations, with 
// the exception of SHIFTing which uses 32 bit shifts and ORS to simulate 64 bit shifting.
    
    uint64_t temporary_sequence_x1x2;   // Corresponds to first 64 bits of the current 64 bit frame.
    uint32_t temporary_sequence_x1;		// Corresponds to first 32 bits of the current 64 bit frame. 			
    uint32_t temporary_sequence_x2;		// Corresponds to second 32 bits of the current 64 bit frame.	
    uint32_t temporary_sequence_x3;     // Corresponds to first 32 bits of the next 64 bit frame.
    
    // Iterates 7 times to generate the 1-7 bit shifted copies of the 48 byte scrambling sequence
    for (i = 1; i< NUM_SHIFTED_SEQUENCES; i++) {
	    uint64_t current_sequence;
	    uint64_t *write_address = (uint64_t *)(bit_aligned_scrambling_sequences[i]);

	    // Iterates through each 64 bit frame of the 48 byte sequences, shifts each frame left by i,
	    // and writes the result into the scrambling sequences 2d array.
	    for (j = 0; j < ((BIT_ALIGNED_SEQUENCE_LENGTH_IN_BYTES>>3) - 1) * 2; j+=2) { 
		    
		    // 64 bit shifts implemented with following formula:
		    // n1 = [0 - 31] n2 = [32 - 63] n3 = [64 - 95]
		    // (n1,n2) << i = ( n1 << i | n2 >> (32-i)  ,  n2 << i | n3 >> (32-i) )
		    // where i is 1-7.    
		    temporary_sequence_x1x2 = _amem8(&(((uint32_t*) bit_aligned_scrambling_sequences[0])[j]));
		    temporary_sequence_x1 = _hill(temporary_sequence_x1x2); 
		    temporary_sequence_x2 = _loll(temporary_sequence_x1x2);
		    temporary_sequence_x3 = _amem4(&((uint32_t*) bit_aligned_scrambling_sequences[0])[j+2]);
		    
		    temporary_sequence_x1 = temporary_sequence_x1 << i;
		    temporary_sequence_x3 = temporary_sequence_x3 >> (32-i);
		    
		    // 32 bit results from or'ing shifted temporary sequences are concatenated into 64 bit current sequence
		    current_sequence = _itoll(temporary_sequence_x1 | (temporary_sequence_x2 >> (32-i)), 
									    (temporary_sequence_x2 << i) | temporary_sequence_x3);

		    // 64 bit current sequence written into scrambling sequences 2d array
		    // write address incremented by 64 bits
		    _amem8(write_address++) = current_sequence;
		    //_mem8(write_address) = current_sequence;
		    //write_address+=2;
	    }

	    // For last 64 bit frame, we do not need a copy of next 64 bit frame. simply OR 32 bit result
	    // of x1 and x2 with 0's.
	    temporary_sequence_x1x2 = 
	    	_amem8(&(((uint32_t*) bit_aligned_scrambling_sequences[0])[(BIT_ALIGNED_SEQUENCE_LENGTH_IN_BYTES>>3)*2-1]));
	    temporary_sequence_x1 = _hill(temporary_sequence_x1x2); 
	    temporary_sequence_x2 = _loll(temporary_sequence_x1x2);

	    temporary_sequence_x1 = temporary_sequence_x1 << i;
	    temporary_sequence_x2 = temporary_sequence_x2 >> (32-i);

	    current_sequence = temporary_sequence_x1 | temporary_sequence_x2;
	    current_sequence |= 0;
	    //_amem8(write_address) = current_sequence;
	    _mem8(write_address) = current_sequence;
    }


    //Retain first byte
    outState->first_byte = first_byte;

    // Initialize bits scrambled to 0
    outState->n_bits_scrambled = 0;
}
