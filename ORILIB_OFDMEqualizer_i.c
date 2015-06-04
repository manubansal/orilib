/**
Atomix project, ORILIB_OFDMEqualizer_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_OFDMEqualizer_algorithms.h"


//void OfdmEqualizer_equalize(
//					IN Cplx16 constPnts[restrict],				//must be 8-byte aligned
//					IN Uint32 length,							//must be even
//					IN Cplx16 signNormalizedPilotPnts[restrict],
//
//					OUT Cplx16 equalizedPnts[restrict],
//					OUT Cplx32 yhstarArray[restrict],
//					INOUT OfdmEqualizerWifiState * state
//					){
void ORILIB_OFDMEqualizer_i (
		IN  ORILIB_t_Cplx16Buf48			* bufInpConstPnts,
		IN  ORILIB_t_Cplx16Buf4				* bufInpSignNormalizedPilotPnts,
		IN  ORILIB_t_ChEstimatorLTF_State		* bufInpChState,
		OUT ORILIB_t_Cplx16Buf48			* bufOutEqualizedPnts,
		OUT ORILIB_t_Cplx32Buf48			* bufOutyhstar,
		OUT ORILIB_t_OFDMEqualizer_State		* bufOutEqState
	){

	Cplx16 *constPnts 			= bufInpConstPnts->samples;
	Cplx16 *signNormalizedPilotPnts 	= bufInpSignNormalizedPilotPnts->samples;
	Cplx16 *equalizedPnts 			= bufOutEqualizedPnts->samples;
	Cplx32 *yhstarArray 			= bufOutyhstar->samples;

	Uint32 length = 48;

	Cplx16  equalizedPnts_Pilots[4];
	Cplx32  yhstarArray_Pilots[4];

	Cplx16U constPntsPhaseRemoved[48];

	Cplx16U pilot_phase_multiplier;

	//TOREMOVE
	//ORILIB_OFDMEqualizer_internal_removeComplexChannel(
	//		(Cplx16U *)signNormalizedPilotPnts,
	//		4,

	//		equalizedPnts_Pilots,
	//		yhstarArray_Pilots,

	//		bufInpChState->avgChannelGainsConj_Pilots,
	//		bufInpChState->recip_hhstarF_Pilots,

	//		&(bufOutEqState->yhstar_norm_min)
	//		);

	ORILIB_OFDMEqualizer_internal_removeComplexChannel_fixedpoint(
			(Cplx16U *)signNormalizedPilotPnts,
			4,

			equalizedPnts_Pilots,
			yhstarArray_Pilots,

			bufInpChState->avgChannelGainsConj_Pilots,
			bufInpChState->recip_hhstar_frac_Pilots,
			bufInpChState->recip_hhstar_exp_Pilots,

			&(bufOutEqState->yhstar_norm_min)
			);



	//at this point, h and y have the same Q-value, whatever it is,
	//but high enough (close to 12-13) due to AGC control, say Q-y.

	ORILIB_OFDMEqualizer_internal_commonPilotPhase_32(
			equalizedPnts_Pilots,

			&pilot_phase_multiplier
			);

	//pilot_phase_multiplier is Q15

	ORILIB_OFDMEqualizer_internal_removeCommonPilotPhase(
			constPnts,
			length,
			pilot_phase_multiplier,
			constPntsPhaseRemoved
			);

	//constPntsPhaseRemoved are again Q-y, so the norm is preserved, and we
	//match to pilots scales da da da.


	//TOREMOVE
	//ORILIB_OFDMEqualizer_internal_removeComplexChannel(
	//		constPntsPhaseRemoved,
	//		length,

	//		equalizedPnts,
	//		yhstarArray,

	//		bufInpChState->avgChannelGainsConj,
	//		bufInpChState->recip_hhstarF,

	//		&bufOutEqState->yhstar_norm_min
	//		);

	ORILIB_OFDMEqualizer_internal_removeComplexChannel_fixedpoint(
			constPntsPhaseRemoved,
			length,

			equalizedPnts,
			yhstarArray,

			bufInpChState->avgChannelGainsConj,
			bufInpChState->recip_hhstar_frac,
			bufInpChState->recip_hhstar_exp,

			&bufOutEqState->yhstar_norm_min
			);




	DEBUG_DATA (
	LOG_PRINTF("data subcarriers after channel equalization (const points)\n");
	printCplx16SingleColumn(bufOutEqualizedPnts->samples, 48);

	LOG_PRINTF("yhstar array\n");
	printCplx32SingleColumn(bufOutyhstar, 48);
	)


}
