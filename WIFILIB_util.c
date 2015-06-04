/**
Atomix project, WIFILIB_util.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>

void WIFILIB_fftShift64(
	Cplx16 const * const restrict symbol_with_DC_at_idx_0,
	Cplx16 * const restrict symbol_with_DC_in_middle
	){

	DEBUG(
	ASSERT_PTR_ALIGNED(symbol_with_DC_at_idx_0, 8);
	ASSERT_PTR_ALIGNED(symbol_with_DC_in_middle, 8);
	)

	Uint32 i;

	{
		Cplx16 * const restrict dst = symbol_with_DC_in_middle;
		Cplx16 const * const restrict src = symbol_with_DC_at_idx_0 + 38;
#define TRIPCOUNT1  13
#pragma MUST_ITERATE(TRIPCOUNT1, TRIPCOUNT1, TRIPCOUNT1);
		for (i = 0; i < TRIPCOUNT1; i++) {
			_amem8(&((Uint64 *)dst)[i]) = _amem8(&((Uint64 *)src)[i]);
		}
	}

	{
		Cplx16 * const restrict dst = symbol_with_DC_in_middle + 26;
		Cplx16 const * const restrict src = symbol_with_DC_at_idx_0 + 1;
#define TRIPCOUNT2  26
#pragma MUST_ITERATE(TRIPCOUNT2, TRIPCOUNT2, TRIPCOUNT2);
		for (i = 0; i < TRIPCOUNT2; i++) {
			_amem4(&((Uint32 *)dst)[i]) = _amem4(&((Uint32 *)src)[i]);
		}
	}
}
