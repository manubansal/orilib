/**
Atomix project, ORILIB_ChEstimatorLTF_algorithms.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_OFDMEQUALIZERLTFCHEST_ALGORITHMS_H_

#include <osl/inc/swpform.h>

void ORILIB_ChEstimatorLTF_internal_estimateFromLtf(
	IN 	Cplx16 	tOfdmBlock[restrict], //accept 160 samples - the full 2xLTF
	OUT Cplx16 	subCarrierGains[restrict], //generate 104 outputs - the first ltf and the second ltf of estimates
	IN	int16_t const * const restrict twiddle_factors_w64
	);

/* Returns a floating point value of recip_hhstar where the true value of this reciprocal
 * is recip_hhstar_frac(Q15) * 2^recip_hhstar_exp.
 *
 * Returns a fixed point hhstar which is Q30 if input channel gains are interpreted as Q15.
 *
 * This new version uses the floating point unit.
 */
void ORILIB_ChEstimatorLTF_internal_channelGainsAverage_floating_point(
			IN  Cplx16 channelGains1[restrict], 	//must be 4-byte aligned
			IN  Cplx16 channelGains2[restrict],		//must be 4-byte aligned
			OUT Cplx16 avgChannelGains[restrict],
			OUT Cplx16 avgChannelGainsConj[restrict],
			OUT Uint32 hhstar[restrict],
			OUT	float recip_hhstarF[restrict],
			IN  Uint32 size							//must be even
			);

/* Returns a floating point value of recip_hhstar where the true value of this reciprocal
 * is recip_hhstar_frac(Q15) * 2^recip_hhstar_exp.
 * 
 * Returns a fixed point hhstar which is Q30 if input channel gains are interpreted as Q15.
 */
void ORILIB_ChEstimatorLTF_internal_channelGainsAverage_fixed_point(
			IN  Cplx16 channelGains1[restrict], 	//must be 4-byte aligned
			IN  Cplx16 channelGains2[restrict],		//must be 4-byte aligned
			OUT Cplx16 avgChannelGains[restrict],
			OUT Cplx16 avgChannelGainsConj[restrict],
			OUT Uint32 hhstar[restrict],
			OUT	Int16 recip_hhstar_frac[restrict],
			OUT Int16 recip_hhstar_exp[restrict],
			IN  Uint32 size							//must be even
			);


void ORILIB_ChEstimatorLTF_internal_channelGainsAvgShiftSignfix(
                        IN  Cplx16 const * const restrict channelGains1raw, 	
                                //must be 8-byte aligned, 64 long

                        IN  Cplx16 const * const restrict channelGains2raw,	
                                //must be 8-byte aligned, 64 long

                        IN  Cplx16 const * const restrict reference_pilot_sequence,

                        OUT Cplx16 * const restrict avgChannelGains		
                                //must be 8-byte aligned, 64 long
                        );

//all arrays are 52 long
void ORILIB_ChEstimatorLTF_internal_channelInverse(
			IN  Cplx16 const * const restrict avgChannelGains,
			OUT Cplx16 * const restrict avgChannelGainsConj,
			OUT Uint32 * const restrict hhstar,
			OUT Uint16 * const restrict recip_hhstar_frac,
			OUT Int16 * const restrict recip_hhstar_exp
			) ;


void ORILIB_ChEstimatorLTF_internal_save(
	IN  Cplx16 const * const restrict avgChannelGains_temp,
	IN  Cplx16 const * const restrict avgChannelGainsConj_temp,
	IN  Uint32 const * const restrict hhstar_temp,
	IN  Uint16 const * const restrict recip_hhstar_frac_temp,
	IN  Int16  const * const restrict recip_hhstar_exp_temp,

	OUT Cplx16 * const restrict avgChannelGains,
	OUT Cplx16 * const restrict avgChannelGainsConj,
	OUT Uint32 * const restrict hhstar,
	OUT Uint16 * const restrict recip_hhstar_frac,
	OUT Int16  * const restrict recip_hhstar_exp,

	OUT Cplx16 * const restrict avgChannelGains_Pilots,
	OUT Cplx16 * const restrict avgChannelGainsConj_Pilots,
	OUT Uint32 * const restrict hhstar_Pilots,

	OUT Uint16 * const restrict recip_hhstar_frac_Pilots,
	OUT Int16  * const restrict recip_hhstar_exp_Pilots
	);

#endif

