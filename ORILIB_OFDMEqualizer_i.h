/**
Atomix project, ORILIB_OFDMEqualizer_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_OFDMEQUALIZER_I_H_
#define ORILIB_OFDMEQUALIZER_I_H_

#include "ORILIB_OFDMEqualizer_t.h"

void ORILIB_OFDMEqualizer_i (
		IN  ORILIB_t_Cplx16Buf48			* bufInpConstPnts,
		IN  ORILIB_t_Cplx16Buf4				* bufInpSignNormalizedPilotPnts,
		IN  ORILIB_t_ChEstimatorLTF_State		* bufInpChState,
		OUT ORILIB_t_Cplx16Buf48			* bufOutEqualizedPnts,
		OUT ORILIB_t_Cplx32Buf48			* bufOutyhstar,
		OUT ORILIB_t_OFDMEqualizer_State		* bufOutEqState
	);

#endif

