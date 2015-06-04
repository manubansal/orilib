/**
Atomix project, ORILIB_CFOCorrectorG_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_CFOCORRECTORG_I_H_

#include <osl/inc/swpform.h>
#include "ORILIB_CFOCorrectorG_t.h"

void ORILIB_CFOCorrectorG_i(
		IN	ORILIB_t_Cplx16Buf80 	*inBuf,
		IN	ORILIB_t_CFOState 	*cfoState,
		IN	ORILIB_t_PktAgcState 	*agcState, 
		OUT	ORILIB_t_Cplx16Buf80 	*outBuf
		);

#endif

