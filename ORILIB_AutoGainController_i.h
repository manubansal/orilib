/**
Atomix project, ORILIB_AutoGainController_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_AUTOGAINCONTROLLER_I_H
#define ORILIB_AUTOGAINCONTROLLER_I_H 

#include "ORILIB_t.h"

void ORILIB_AutoGainController_i (
		IN  ORILIB_t_Cplx16Buf80 const 	* const restrict unalignedRawSampleBuf,
		IN  ORILIB_t_SimpleEnergyState 	* inoutSimpleEnergyState,
		IN  ORILIB_t_GainState 		* inoutGainState,
		IN  ORILIB_t_GainStateAux 	* inoutGainStateAux,
		OUT ORILIB_t_Cplx16Buf80 	* const restrict unalignedRawSampleBufWithGain
	);

#endif /* ORILIB_AUTOGAINCONTROLLER_I_H */
