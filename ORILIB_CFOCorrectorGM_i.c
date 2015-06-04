/**
Atomix project, ORILIB_CFOCorrectorGM_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_CFOCorrectorGM_t.h"

#include "ORILIB_utils_inl.h"
#include "WIFILIB_lookupTables.h"
//#include "IQmath_inline_all.h"
#include "IQmath_inline.h"
#include "WIFILIB_util.h"

#include "ORILIB_CFOCorrector_algos.h"


//ApplyGainAndCorrectCfoAndMultiplyFactor
void ORILIB_CFOCorrectorGM_i(
		IN	ORILIB_t_Cplx16Buf80 	*inBuf,
		IN	ORILIB_t_CFOState 	*cfoState,
		IN	ORILIB_t_PktAgcState 	*agcState, 
		OUT	ORILIB_t_Cplx16Buf80 	*outBuf
		) {

	Cplx16 *inSampleBuf 	= inBuf->samples;
	Cplx16 *outSampleBuf 	= outBuf->samples;

	ORILIB_CFOCorrectorGM_inner(
		inSampleBuf,
		cfoState,
		agcState,
		outSampleBuf
		);
}
