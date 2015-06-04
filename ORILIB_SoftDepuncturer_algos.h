/**
Atomix project, ORILIB_SoftDepuncturer_algos.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_SOFTDEPUNCTURER_ALGOS_H_
#define ORILIB_SOFTDEPUNCTURER_ALGOS_H_

#include "ORILIB_t.h"

void ORILIB_SoftDepuncturerThreeFourths(	
		IN 	BitsSoft const * const restrict bits_soft,
		OUT 	BitsSoft  * const restrict depunctured_soft_bits, 
		IN	Uint32 num_coded_bits	//before depuncturing
		);

#endif

