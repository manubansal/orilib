/**
Atomix project, ORILIB_SoftDemapperBPSKyhstar_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_SOFTDEMAPPERBPSKYHSTAR_I_H_

#include "ORILIB_SoftDemapperBPSKyhstar_t.h"

void ORILIB_SoftDemapperBPSKyhstar_i (
		IN  ORILIB_t_Cplx32Buf48		* inpBufyhstar,
		IN  ORILIB_t_OFDMEqualizer_State	* inpBufEqState,
		OUT ORILIB_t_SoftBitBuf48		* outBufSoftBits
	);

#endif

