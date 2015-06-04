/**
Atomix project, ORILIB_SoftDeinterleaver_algorithms.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_SOFTDEINTERLEAVER_ALGORITHMS_H_

#include <osl/inc/swpform.h>
#include <osl/inc/or_types.h>

void ORILIB_SoftDeinterleaver_lut_implementation (
	IN 	SoftBit const * const restrict interleaved_bits_in, 
	IN	Uint32 num_bits,
	IN	Uint16 const * const restrict lut_pointer,
	OUT 	SoftBit * const restrict deinterleaved_bits_out
	);

#endif

