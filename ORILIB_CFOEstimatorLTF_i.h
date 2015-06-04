/**
Atomix project, ORILIB_CFOEstimatorLTF_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_CFOESTIMATORLTF_I_H_

#include "ORILIB_t.h"

//allLtfSamples_noGain		//the entire ltf portion, from the start, including the ltf's cp etc.
//phiU				//cfo unit angle
//sign_cfo			//1 if negative, 0 o.w.
//derot_table_stride
//derotTablePtr


void ORILIB_CFOEstimatorLTF_i (
		IN	ORILIB_t_Cplx16Buf160 const * const inpFullLtfBuf,
		IN	ORILIB_t_PktAgcState const * const restrict agcState,
		OUT	ORILIB_t_CFOState * const restrict cfoState
		);

		//OUT	Uint32 * const restrict phiU,
		//OUT	Uint32 * const restrict sign_cfo,
		//OUT	Uint32 * const restrict derot_table_stride,
		//OUT	Cplx16U const ** const restrict derotTablePtr

#endif
