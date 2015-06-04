/**
Atomix project, ORILIB_OFDMModulator_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_OFDMMODULATOR_I_H_

#include <osl/inc/swpform.h>
#include "ORILIB_OFDMModulator_t.h"

void ORILIB_OFDMModulator_i (
	IN 	ORILIB_t_Cplx16Buf48 		* inpSampleBuf,
	IN	ORILIB_OFDMModulator_t_State 	* inpStateBuf,
	OUT	ORILIB_t_Cplx16Buf80 		* outSampleBuf,
	OUT	ORILIB_OFDMModulator_t_State 	* outStateBuf,
	CF	ORILIB_OFDMModulator_t_Conf	* conf
	);


void ORILIB_OFDMModulator_i_conf(
	CF ORILIB_OFDMModulator_t_Conf * conf,
	Uint32 scaleFactor
	);

#endif

