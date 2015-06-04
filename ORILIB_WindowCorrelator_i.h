/**
Atomix project, ORILIB_WindowCorrelator_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_WINDOWCORRELATOR_I_H 

#include "ORILIB_t.h"

void ORILIB_WindowCorrelator_i(
		IN  ORILIB_t_Cplx16Buf80 const 	* const restrict sampleBufWithGain,
		IN  ORILIB_t_GainStateAux 	* 		inpGainState,
		IN  ORILIB_t_AlignState 	* 		inoutAlignState,
		IN  ORILIB_t_CorrState 		* 		inoutCorrState,
		OUT ORILIB_t_CorrStateAux 	* 		inoutCorrStateAux
		);

#endif /* ORILIB_WINDOWCORRELATOR_I_H */
