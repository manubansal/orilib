/**
Atomix project, ORILIB_CFOCorrectorG_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_CFOCorrectorG_t.h"
#include "WIFILIB_util.h"

#include "ORILIB_CFOCorrector_algos.h"

//ApplyGainAndCorrectCfo
void ORILIB_CFOCorrectorG_i(
		IN	ORILIB_t_Cplx16Buf80 	*inBuf,
		IN	ORILIB_t_CFOState 	*cfoState,
		IN	ORILIB_t_PktAgcState 	*agcState, 
		OUT	ORILIB_t_Cplx16Buf80 	*outBuf
		) {

	Cplx16 *inSampleBuf 	= inBuf->samples;
	Cplx16 *outSampleBuf 	= outBuf->samples;

	ORILIB_CFOCorrectorG_inner(
		inSampleBuf,
		cfoState,
		agcState,
		outSampleBuf
		);
}

