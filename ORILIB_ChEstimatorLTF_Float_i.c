/**
Atomix project, ORILIB_ChEstimatorLTF_Float_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <osl/inc/util.h>


#include "ORILIB_ChEstimatorLTF_Float_t.h"
#include "ORILIB_ChEstimatorLTF_algorithms.h"



extern Int16 ORILIB_twiddle_factors_fft16x16_64[128];

//IN  Cplx16 	tOfdmBlock[restrict], //accept 160 samples - the full 2xLTF
//INOUT OfdmEqualizerWifiState * const state

far static Int16 recip_hhstar_frac_temp[52]; 
far static Int16 recip_hhstar_exp_temp[52];

void ORILIB_ChEstimatorLTF_Float_i (
		IN  ORILIB_t_Cplx16Buf160 * restrict inpLtfsBuf,
		OUT ORILIB_t_ChEstimatorLTF_Float_State * state
		) {

	OUT Cplx16 * restrict avgChannelGains;
	OUT Cplx16 * restrict avgChannelGainsConj;
	OUT Uint32 * restrict hhstar;
//	OUT Int16  * restrict recip_hhstar_frac;
//	OUT Int16  * restrict recip_hhstar_exp;
	OUT float  * restrict recip_hhstarF;


	OUT Cplx16 * restrict avgChannelGains_Pilots;
	OUT Cplx16 * restrict avgChannelGainsConj_Pilots;
	OUT Uint32 * restrict hhstar_Pilots;
	OUT float  * restrict recip_hhstarF_Pilots;


	Cplx16 subCarrierGains[104];
	Cplx16 avgChannelGains_temp[52], avgChannelGainsConj_temp[52];
	Uint32 hhstar_temp[52];
	float recip_hhstarF_temp[52];

	Cplx16 * tOfdmBlock = inpLtfsBuf->samples;
	
	avgChannelGains			= state->avgChannelGains;
	avgChannelGainsConj 		= state->avgChannelGainsConj;
	hhstar 				= state->hhstar;
//	recip_hhstar_frac		= state->recip_hhstar_frac;
//	recip_hhstar_exp		= state->recip_hhstar_exp;
	recip_hhstarF			= state->recip_hhstarF;

	avgChannelGains_Pilots		= state->avgChannelGains_Pilots;
	avgChannelGainsConj_Pilots	= state->avgChannelGainsConj_Pilots;
	hhstar_Pilots 			= state->hhstar_Pilots;
	recip_hhstarF_Pilots		= state->recip_hhstarF_Pilots;

	ORILIB_ChEstimatorLTF_internal_estimateFromLtf(
			tOfdmBlock+32, //accept 160 samples - the full 2xLTF
			subCarrierGains, //generate 104 outputs - the first ltf and the second ltf of estimates
			ORILIB_twiddle_factors_fft16x16_64
			);
	
	ORILIB_ChEstimatorLTF_internal_estimateFromLtf(
			tOfdmBlock+96, //accept 160 samples - the full 2xLTF
			subCarrierGains+52, //generate 104 outputs - the first ltf and the second ltf of estimates
			ORILIB_twiddle_factors_fft16x16_64
			);

	ORILIB_ChEstimatorLTF_internal_channelGainsAverage_floating_point(
			subCarrierGains, 	//must be 4-byte aligned
			subCarrierGains + 52,		//must be 4-byte aligned
			avgChannelGains_temp,
			avgChannelGainsConj_temp,
			hhstar_temp,
			recip_hhstarF_temp,
			52						//must be even
			);


	/*---------------- Strip pilots, save data subcarriers. ---------------------*/
	memcpy(avgChannelGains,		avgChannelGains_temp	,5*sizeof(Cplx16));
	memcpy(avgChannelGains + 5,avgChannelGains_temp+6	,13*sizeof(Cplx16));
	memcpy(avgChannelGains + 18,avgChannelGains_temp+20 ,12*sizeof(Cplx16));
	memcpy(avgChannelGains + 30,avgChannelGains_temp+33 ,13*sizeof(Cplx16));
	memcpy(avgChannelGains + 43,avgChannelGains_temp+47	,5*sizeof(Cplx16));
	
	memcpy(avgChannelGainsConj,avgChannelGainsConj_temp,5*sizeof(Cplx16));
	memcpy(avgChannelGainsConj + 5,avgChannelGainsConj_temp+6,13*sizeof(Cplx16));
	memcpy(avgChannelGainsConj + 18,avgChannelGainsConj_temp+20,12*sizeof(Cplx16));
	memcpy(avgChannelGainsConj + 30,avgChannelGainsConj_temp+33,13*sizeof(Cplx16));
	memcpy(avgChannelGainsConj + 43,avgChannelGainsConj_temp+47,5*sizeof(Cplx16));
	
	memcpy(hhstar,hhstar_temp,5*sizeof(Uint32));
	memcpy(hhstar + 5,hhstar_temp+6,13*sizeof(Uint32));
	memcpy(hhstar + 18,hhstar_temp+20,12*sizeof(Uint32));
	memcpy(hhstar + 30,hhstar_temp+33,13*sizeof(Uint32));
	memcpy(hhstar + 43,hhstar_temp+47,5*sizeof(Uint32));
	
	memcpy(recip_hhstarF,recip_hhstarF_temp,5*sizeof(float));
	memcpy(recip_hhstarF + 5,recip_hhstarF_temp+6,13*sizeof(float));
	memcpy(recip_hhstarF + 18,recip_hhstarF_temp+20,12*sizeof(float));
	memcpy(recip_hhstarF + 30,recip_hhstarF_temp+33,13*sizeof(float));
	memcpy(recip_hhstarF + 43,recip_hhstarF_temp+47,5*sizeof(float));

	/*--------------- Save the pilots separately. ---------------*/
	avgChannelGains_Pilots[0] = avgChannelGains_temp[5];
	avgChannelGains_Pilots[1] = avgChannelGains_temp[19];
	avgChannelGains_Pilots[2] = avgChannelGains_temp[32];
	avgChannelGains_Pilots[3] = avgChannelGains_temp[46];

	avgChannelGainsConj_Pilots[0] = avgChannelGainsConj_temp[5];
	avgChannelGainsConj_Pilots[1] = avgChannelGainsConj_temp[19];
	avgChannelGainsConj_Pilots[2] = avgChannelGainsConj_temp[32];
	avgChannelGainsConj_Pilots[3] = avgChannelGainsConj_temp[46];

	hhstar_Pilots[0] = hhstar_temp[5];
	hhstar_Pilots[1] = hhstar_temp[19];
	hhstar_Pilots[2] = hhstar_temp[32];
	hhstar_Pilots[3] = hhstar_temp[46];

	recip_hhstarF_Pilots[0] = recip_hhstarF_temp[5];
	recip_hhstarF_Pilots[1] = recip_hhstarF_temp[19];
	recip_hhstarF_Pilots[2] = recip_hhstarF_temp[32];
	recip_hhstarF_Pilots[3] = recip_hhstarF_temp[46];

	//DEBUG(
	//LOG_PRINTF("52 average channel gains, before separating data and pilots\n");
	//printCplx16SingleColumn(avgChannelGains_temp, 52);
	//)

	DEBUG_DATA(LOG_PRINTF("recip_hhstarF_Pilots[0]: %f\n", recip_hhstarF_Pilots[0]);)
	DEBUG_DATA(LOG_PRINTF("recip_hhstarF_Pilots[1]: %f\n", recip_hhstarF_Pilots[1]);)
	DEBUG_DATA(LOG_PRINTF("recip_hhstarF_Pilots[2]: %f\n", recip_hhstarF_Pilots[2]);)
	DEBUG_DATA(LOG_PRINTF("recip_hhstarF_Pilots[3]: %f\n", recip_hhstarF_Pilots[3]);)
}
