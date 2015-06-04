/**
Atomix project, ORILIB_BitCacheInit_circular_C1024_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_BitCacheInit_circular_C1024_t.h"

void ORILIB_BitCacheInit_circular_C1024_i(
	OUT ORILIB_t_BitCache1024_circular	* outCache
	) {

	outCache->validLenInBits = 0;
	outCache->validBitsOffset = 0;
}
