/**
Atomix project, ORILIB_OFDMEqualizer_algorithms.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_OFDMEQUALIZER_ALGORITHMS_H_

#include <osl/inc/swpform.h>

void ORILIB_OFDMEqualizer_internal_commonPilotPhase_32(
		IN		Cplx16 *equalizedPilotPnts,
		OUT		Cplx16U *pilot_phase_multiplier
		);

void ORILIB_OFDMEqualizer_internal_removeCommonPilotPhase(
		IN Cplx16 constPnts[restrict],
		IN Uint32 length,
		IN Cplx16U pilot_phase_multiplier,

		OUT Cplx16U constPntsPhaseRemoved[restrict]
		);

void ORILIB_OFDMEqualizer_internal_removeComplexChannel(
		IN Cplx16U constPnts[restrict],				//must be 8-byte aligned
		IN Uint32 length,							//must be a multiple of 4

		OUT Cplx16 equalizedPnts[restrict],
		OUT Cplx32 yhstarArray[restrict],

		IN	Cplx16 hstar[restrict],					//channel conjugate values
		IN	float rhhstar[restrict],				//reciprocal hhstar values

		OUT	Uint32 *yhstar_norm_min						//minimum norm of any I or Q component of yhstar values,
													//used later by soft demapper for scaling yhstar values
		);


void ORILIB_OFDMEqualizer_internal_removeComplexChannel_fixedpoint(
		IN Cplx16U constPnts[restrict],				//must be 8-byte aligned
		IN Uint32 length,							//must be a multiple of 4

		OUT Cplx16 equalizedPnts[restrict],
		OUT Cplx32 yhstarArray[restrict],

		IN	Cplx16 hstar[restrict],					//channel conjugate values
		IN	Uint16 rhhstar_frac[restrict],				//reciprocal hhstar values
		IN	Int16 rhhstar_exp[restrict],				//reciprocal hhstar values

		OUT	Uint32 *yhstar_norm_min						//minimum norm of any I or Q component of yhstar values,
													//used later by soft demapper for scaling yhstar values
		);

#endif

