/**
Atomix project, ORILIB_PeakDetector_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_PEAKDETECTOR_I_H
#define ORILIB_PEAKDETECTOR_I_H 

#include "ORILIB_t.h"

void ORILIB_PeakDetector_i (
		IN  	ORILIB_t_Cplx16Buf80 const 	* const restrict sampleBufWithGain,
		IN	ORILIB_t_GainStateAux * gainStateAux,
		IN 	ORILIB_t_AlignState * alignState,
		IN 	ORILIB_t_CorrState * corrState,
		OUT 	ORILIB_t_DetectState * outDetectState
		);

#endif /* ORILIB_PEAKDETECTOR_I_H */
