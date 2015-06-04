/**
Atomix project, ORILIB_OFDMDemodulator_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_OFDMDEMODULATOR_I_H_

#include <osl/inc/swpform.h>
#include "ORILIB_OFDMDemodulator_t.h"

void ORILIB_OFDMDemodulator_i (
	IN 	ORILIB_t_Cplx16Buf80 		* inpSampleBuf,
	IN	ORILIB_OFDMDemodulator_t_State 	* inpStateBuf,
	OUT	ORILIB_t_Cplx16Buf48 		* outDataSubcarriersBuf,
	OUT	ORILIB_t_Cplx16Buf4  		* outPilotSubcarriersBuf,
	OUT	ORILIB_OFDMDemodulator_t_State 	* outStateBuf
	);

#endif

