/**
Atomix project, ORILIB_Descrambler_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_DESCRAMBLER_T_H_
#define ORILIB_DESCRAMBLER_T_H_

#include <osl/inc/swpform.h>
#include <osl/inc/or_types.h>
#include "ORILIB_t.h"


#define PRIMARY_SCRAMBLER_SEQUENCE_LENGTH_IN_BITS 127 /**< length in bits of the IEEE defined scrambling sequence*/
#define BIT_ALIGNED_SEQUENCE_LENGTH_IN_BYTES 48 /**< length in bytes of the repeating sequence defined scrambling sequence*/
#define NUM_SHIFTED_SEQUENCES 8
#define BYTE_ALIGNMENT 8


typedef struct ORILIB_t_Descrambler_State_s {
	uint8_t extendedDescramblerSequence[8][48] __attribute__((aligned(8)));
	uint32_t n_bits_scrambled;
	uint8_t first_byte;
} ORILIB_t_Descrambler_State;

typedef struct ORILIB_t_DescramblerConf_s {
	uint32_t data_length_in_bits;
	uint32_t n_double_words;
} ORILIB_t_DescramblerConf;

#endif

