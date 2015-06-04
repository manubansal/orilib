/**
Atomix project, ORILIB_ChEstimatorLTF_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include <osl/inc/swpform.h>
#include <osl/inc/util.h>


#include "ORILIB_ChEstimatorLTF_t.h"
#include "ORILIB_ChEstimatorLTF_algorithms.h"

extern Int16 ORILIB_twiddle_factors_fft16x16_64[128];

//accept 160 samples - the full 2xLTF

void ORILIB_ChEstimatorLTF_i (
		IN  ORILIB_t_Cplx16Buf160 * restrict inpLtfsBuf,
		OUT ORILIB_t_ChEstimatorLTF_State * state
		) {

	OUT Cplx16 * restrict avgChannelGains;
	OUT Cplx16 * restrict avgChannelGainsConj;
	OUT Uint32 * restrict hhstar;
	OUT Uint16 * restrict recip_hhstar_frac;
	OUT Int16  * restrict recip_hhstar_exp;

	OUT Cplx16 * restrict avgChannelGains_Pilots;
	OUT Cplx16 * restrict avgChannelGainsConj_Pilots;
	OUT Uint32 * restrict hhstar_Pilots;

	OUT Uint16 * restrict recip_hhstar_frac_Pilots;
	OUT Int16  * restrict recip_hhstar_exp_Pilots;

	Cplx16 subCarrierGains[128];
	Cplx16 avgChannelGains_temp[52]; 
	Cplx16 avgChannelGainsConj_temp[52];
	Uint32 hhstar_temp[52];
	Uint16 recip_hhstar_frac_temp[52]; 
	Int16 recip_hhstar_exp_temp[52];

	Cplx16 * tOfdmBlock 		= inpLtfsBuf->samples;
	
	avgChannelGains			= state->avgChannelGains;
	avgChannelGainsConj 		= state->avgChannelGainsConj;
	hhstar 				= state->hhstar;
	recip_hhstar_frac		= state->recip_hhstar_frac;
	recip_hhstar_exp		= state->recip_hhstar_exp;

	avgChannelGains_Pilots		= state->avgChannelGains_Pilots;
	avgChannelGainsConj_Pilots	= state->avgChannelGainsConj_Pilots;
	hhstar_Pilots 			= state->hhstar_Pilots;
	recip_hhstar_frac_Pilots	= state->recip_hhstar_frac_Pilots;
	recip_hhstar_exp_Pilots		= state->recip_hhstar_exp_Pilots;

	ORILIB_ChEstimatorLTF_internal_estimateFromLtf(
			tOfdmBlock+32,
			subCarrierGains,
			ORILIB_twiddle_factors_fft16x16_64
			);
	
	ORILIB_ChEstimatorLTF_internal_estimateFromLtf(
			tOfdmBlock+96, 
			subCarrierGains+64,
			ORILIB_twiddle_factors_fft16x16_64
			);

	ORILIB_ChEstimatorLTF_internal_channelGainsAvgShiftSignfix(
			subCarrierGains,
			subCarrierGains + 64,
			ORILIB_reference_ltf_d_signs,
			avgChannelGains_temp
			);


	ORILIB_ChEstimatorLTF_internal_channelInverse(
			avgChannelGains_temp,
			avgChannelGainsConj_temp,
			hhstar_temp,
			recip_hhstar_frac_temp,
			recip_hhstar_exp_temp
			);



	/*---------------- Strip pilots, save data subcarriers. ---------------------*/
	ORILIB_ChEstimatorLTF_internal_save(
			avgChannelGains_temp,
			avgChannelGainsConj_temp,
			hhstar_temp,
			recip_hhstar_frac_temp,
			recip_hhstar_exp_temp,

			avgChannelGains,
			avgChannelGainsConj,
			hhstar,
			recip_hhstar_frac,
			recip_hhstar_exp,

			avgChannelGains_Pilots,
			avgChannelGainsConj_Pilots,
			hhstar_Pilots,

			recip_hhstar_frac_Pilots,
			recip_hhstar_exp_Pilots
			);

	//DEBUG(
	//LOG_PRINTF("52 average channel gains, before separating data and pilots\n");
	//printCplx16SingleColumn(avgChannelGains_temp, 52);
	//)

	//DEBUG_DATA(LOG_PRINTF("recip_hhstarF_Pilots[0]: %f\n", recip_hhstarF_Pilots[0]);)
	//DEBUG_DATA(LOG_PRINTF("recip_hhstarF_Pilots[1]: %f\n", recip_hhstarF_Pilots[1]);)
	//DEBUG_DATA(LOG_PRINTF("recip_hhstarF_Pilots[2]: %f\n", recip_hhstarF_Pilots[2]);)
	//DEBUG_DATA(LOG_PRINTF("recip_hhstarF_Pilots[3]: %f\n", recip_hhstarF_Pilots[3]);)
}
