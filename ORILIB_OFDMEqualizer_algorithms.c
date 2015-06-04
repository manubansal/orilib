/**
Atomix project, ORILIB_OFDMEqualizer_algorithms.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

//#define PHY_SOFT_SLICER_UNNORM_UNIT_SCALE_UNBIASED			6		//Q6  this is because the biggest normalized TODO HACK
///#define N_ADDITIONAL_INT_BITS						(8 - PHY_SOFT_SLICER_UNNORM_UNIT_SCALE - 1 - 1)
//#define PHY_SOFT_SLICER_SCALE_BIAS_DUE_TO_PAPR_AGC_MARGIN   		2		//this must match SYNC_AGC_GAIN_SAFETY_MARGIN_FOR_HIGH_PAPR
//#define PHY_SOFT_SLICER_UNNORM_UNIT_SCALE  (PHY_SOFT_SLICER_UNNORM_UNIT_SCALE_UNBIASED + PHY_SOFT_SLICER_SCALE_BIAS_DUE_TO_PAPR_AGC_MARGIN)
#define PHY_SOFT_SLICER_UNNORM_UNIT_SCALE  6
//#define PHY_SOFT_SLICER_UNNORM_UNIT_SCALE  7
#define FIXED_PT_RIGHT_SHIFT  (16 - PHY_SOFT_SLICER_UNNORM_UNIT_SCALE)


void ORILIB_OFDMEqualizer_internal_commonPilotPhase_32(
		IN		Cplx16 *equalizedPilotPnts,
		OUT		Cplx16U *pilot_phase_multiplier
		){

	/**********************************************************
	 * Pilot phase tracking - find the average phase factor
	 * that will be removed from data symbols while equalizing.
	 *********************************************************/
	Cplx16U p0, p1, p2, p3;
	Cplx16U h0, h1, h2, h3;
	Cplx32U phs_avg, phs_avg_s1, phs_avg_s2;
	Cplx32U phs0, phs1, phs2, phs3;	//p.h*

	double phs_recip_root_normD;


	p0.realimag = _amem4(&equalizedPilotPnts[0]);
	p1.realimag = _amem4(&equalizedPilotPnts[1]);
	p2.realimag = _amem4(&equalizedPilotPnts[2]);
	p3.realimag = _amem4(&equalizedPilotPnts[3]);


	phs_avg.cplx32.real = p0.cplx16.real + p1.cplx16.real + p2.cplx16.real + p3.cplx16.real;
	phs_avg.cplx32.imag = p0.cplx16.imag + p1.cplx16.imag + p2.cplx16.imag + p3.cplx16.imag;


	//normalize the phase average

	//take out the scalar gain to retain only the phase
	//let's use the fpu -- this can be done with iqmath recip if desired.
	phs_recip_root_normD = _rsqrdp((double)(
			_mpy32ll(phs_avg.cplx32.real, phs_avg.cplx32.real) +
			_mpy32ll(phs_avg.cplx32.imag, phs_avg.cplx32.imag) +
			1
			));
	//+1 is to avoid divide by zero error. it's loss in accuracy is minimal as we are
	//Given we had 8-bit (at most) occupancy in phs_avg real and imag parts, norm will
	//look like 16-bits at most, which is 2^16. norm root is then 2^8. recip is like 2^-8,
	//so multiply by 2^16 to have reasonable available precision in fixed point.
//		phs_recip_root_norm = _spint((1<<16) * phs_recip_root_normF);

	phs_avg.cplx32.real = _dpint((1<<14) * (phs_recip_root_normD * (double)phs_avg.cplx32.real));
	phs_avg.cplx32.imag = _dpint((1<<14) * (phs_recip_root_normD * (double)phs_avg.cplx32.imag));
	//a high-precision and full-scale Cplx16

	phs_avg.cplx32.real = _ext(_sshl(phs_avg.cplx32.real, 17), 0, 16);
	phs_avg.cplx32.imag = _ext(_sshl(-phs_avg.cplx32.imag, 17), 0, 16);


	//pack with conjugation, so that this is the direct term to be multiplied with
//	pilot_phase_multiplier->realimag = _pack2(phs_avg.cplx32.real, -phs_avg.cplx32.imag);
	pilot_phase_multiplier->realimag = _pack2(phs_avg.cplx32.real, phs_avg.cplx32.imag);

	//pilot_phase_multiplier is Q15
}



void ORILIB_OFDMEqualizer_internal_removeCommonPilotPhase(
		IN Cplx16 constPnts[restrict],
		IN Uint32 length,
		IN Cplx16U pilot_phase_multiplier,

		OUT Cplx16U constPntsPhaseRemoved[restrict]
		) {
#ifdef DEBUG_MODE
	assert(length == 48);
#endif

	Uint32 i;
	Cplx16U pnt, mul, pnt_no_phase;

	mul.realimag = pilot_phase_multiplier.realimag;

#pragma MUST_ITERATE(48,48,48);
	for (i = 0; i < 48; i++) {
		pnt.realimag = _amem4(&constPnts[i]);
		pnt_no_phase.realimag = _cmpyr1(pnt.realimag, mul.realimag);
		//Q-y * Q-15 = Q-(y+15) => Q-(y+15+1) due to left shift => Q-(y+15+1-16) = Q-y, awesome!
		_amem4(&constPntsPhaseRemoved[i]) = pnt_no_phase.realimag;
	}
}

//I'm relaxing this function to have an input length that is
//a multiple of 4 but not necessarily 48. This will allow us
//to use the function to equalize pilot symbols too, which is
//needed before estimating the common pilot phase. --MB, 2/15/12
void ORILIB_OFDMEqualizer_internal_removeComplexChannel(
		IN Cplx16U constPnts[restrict],				//must be 8-byte aligned
		IN Uint32 length,							//must be a multiple of 4

		OUT Cplx16 equalizedPnts[restrict],
		OUT Cplx32 yhstarArray[restrict],

		IN	Cplx16 hstar[restrict],					//channel conjugate values
		IN	float rhhstar[restrict],				//reciprocal hhstar values

		OUT	Uint32 *yhstar_norm_min						//minimum norm of any I or Q component of yhstar values,
													//used later by soft demapper for scaling yhstar values
		){

	Uint32 i;
	Cplx16U lo_y, lo_hstar;
//	Cplx16U lo_yhstar;
	Cplx32U lo_yhstar;

	Int32 lo_yhat_i, lo_yhat_q;
	float lo_yhat_i_F, lo_yhat_q_F;
	float lo_yhstar_i_F, lo_yhstar_q_F;
	float lo_rhhstarF;

	float factor = (float)(1<<PHY_SOFT_SLICER_UNNORM_UNIT_SCALE);

	Uint32 lo_norm_min = 32;
	Uint32 norm_I = 32;
	Uint32 norm_Q = 32;

#ifdef DEBUG_MODE
//	assert(length == 48);
	assert((length % 4 == 0));
	ASSERT_PTR_ALIGNED(yhstarArray, 8);
#endif

	//find yhstar first
//#pragma MUST_ITERATE(48, 48, 48);
#pragma MUST_ITERATE(4, , 4);
//	for (i = 0; i < 48; i++) {
	for (i = 0; i < length; i++) {
		lo_y.realimag = _amem4(&constPnts[i]);
		lo_hstar.realimag = _amem4(&hstar[i]);

//		lo_yhstar.realimag = _cmpyr1(lo_y.realimag, lo_hstar.realimag);
//		//Q-y * Q-y => Q-2y => Q-2y + 1 => Q-(2y - 15)
//		_amem4(&hstarArray[i]) = lo_yhstar.realimag;

		lo_yhstar.realimag = _cmpy(lo_y.realimag, lo_hstar.realimag);
		//Q-y * Q-y => Q-2y
		norm_I = _norm(lo_yhstar.cplx32.real);
		norm_Q = _norm(lo_yhstar.cplx32.imag);
		lo_norm_min = lo_norm_min < norm_I ? lo_norm_min : norm_I;
		lo_norm_min = lo_norm_min < norm_Q ? lo_norm_min : norm_Q;

		//yhstarArray has Q-(2y-15) data format now
		//now scale out by hhstar, which is fed in as reciprocals already
		lo_rhhstarF = rhhstar[i];
		lo_yhstar_i_F = (float)lo_yhstar.cplx32.real;
		lo_yhstar_q_F = (float)lo_yhstar.cplx32.imag;
		lo_yhat_i_F = lo_yhstar_i_F * lo_rhhstarF;
		lo_yhat_q_F = lo_yhstar_q_F * lo_rhhstarF;

		lo_yhat_i = _spint(lo_yhat_i_F * factor);
		lo_yhat_q = _spint(lo_yhat_q_F * factor);

		_amem8(&yhstarArray[i]) = lo_yhstar.realimag;
		_amem4(&equalizedPnts[i]) = _pack2(lo_yhat_i, lo_yhat_q);

	}
//	LOG_PRINTF("norm_min: %d", norm_min);
	*yhstar_norm_min = lo_norm_min;


}



////////////////////////////////////// TODO ///////////////////////////////

//I'm relaxing this function to have an input length that is
//a multiple of 4 but not necessarily 48. This will allow us
//to use the function to equalize pilot symbols too, which is
//needed before estimating the common pilot phase. --MB, 2/15/12
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
		){

	Uint32 i;
	Cplx16U lo_y, lo_hstar, lo_yhstar, lo_yhat;
	Cplx32U lo_yhstarC32, lo_yhatC32;

	//float lo_rhhstarF;
	//float lo_yhstar_i_F, lo_yhstar_q_F;
	//float lo_yhat_i_F, lo_yhat_q_F;
	Uint16 lo_rhhstar_frac, lo_rhhstar_exp;
	//Int64 lo_yhat_i_64, lo_yhat_q_64;
	long long lo_yhat_i_64, lo_yhat_q_64;
	Int32 lo_yhat_i, lo_yhat_q;

	Cplx16U lo_rhhstar_frac_w;

	//float factor = (float)(1<<PHY_SOFT_SLICER_UNNORM_UNIT_SCALE);

	Uint32 lo_norm_min = 32;
	Uint32 norm_I = 32;
	Uint32 norm_Q = 32;
	Uint32 shr;

#ifdef DEBUG_MODE
//	assert(length == 48);
	assert((length % 4 == 0));
	ASSERT_PTR_ALIGNED(yhstarArray, 8);
#endif

	//find yhstar first
//#pragma MUST_ITERATE(48, 48, 48);
#pragma MUST_ITERATE(4, , 4);
//	for (i = 0; i < 48; i++) {
	for (i = 0; i < length; i++) {
		lo_y.realimag = _amem4(&constPnts[i]);
		lo_hstar.realimag = _amem4(&hstar[i]);

		//Step 1
		lo_yhstarC32.realimag = _cmpy(lo_y.realimag, lo_hstar.realimag);
		//Q-y * Q-y => Q-2y

		//Step 2
		norm_I = _norm(lo_yhstarC32.cplx32.real);
		norm_Q = _norm(lo_yhstarC32.cplx32.imag);
		lo_norm_min = lo_norm_min < norm_I ? lo_norm_min : norm_I;
		lo_norm_min = lo_norm_min < norm_Q ? lo_norm_min : norm_Q;
		lo_yhstar.realimag = _packh2(
				 _ext(lo_yhstarC32.cplx32.real, norm_I, 0),
				 _ext(lo_yhstarC32.cplx32.imag, norm_Q, 0)
				 );

		//yhstarArray has Q-(2y-15) data format now
		//now scale out by hhstar, which is fed in as reciprocals already

		//lo_rhhstarF = rhhstar[i];
		//lo_yhstar_i_F = (float)lo_yhstarC32.cplx32.real;
		//lo_yhstar_q_F = (float)lo_yhstarC32.cplx32.imag;
		//lo_yhat_i_F = lo_yhstar_i_F * lo_rhhstarF;
		//lo_yhat_q_F = lo_yhstar_q_F * lo_rhhstarF;

		//lo_yhat_i = _spint(lo_yhat_i_F * factor);
		//lo_yhat_q = _spint(lo_yhat_q_F * factor);

		//Step 3
		//lo_rhhstar_frac = rhhstar_frac[i];
		lo_rhhstar_frac_w.cplx16.real = rhhstar_frac[i];
		lo_rhhstar_frac_w.cplx16.imag = lo_rhhstar_frac_w.cplx16.real;

		lo_rhhstar_exp = rhhstar_exp[i];
		//lo_yhat_i_64 = lo_yhstarC32.cplx32.real * lo_rhhstar_frac;
		//lo_yhat_q_64 = lo_yhstarC32.cplx32.imag * lo_rhhstar_frac;
		lo_yhatC32.realimag = _mpy2ll(lo_rhhstar_frac_w.realimag, lo_yhstar.realimag);

		//Step 4
		//bit-shift according to norm to fit yhat into 16 bit I, 16 bit Q
		//lo_yhatC32.cplx32.real = _ext(lo_yhatC32.cplx32.real, 0,
		//		14 + norm_I - lo_rhhstar_exp + FIXED_PT_RIGHT_SHIFT);
		shr = 14 + norm_I + FIXED_PT_RIGHT_SHIFT - lo_rhhstar_exp; 
		lo_yhatC32.cplx32.real = lo_yhatC32.cplx32.real >> shr;

		//lo_yhatC32.cplx32.imag = _ext(lo_yhatC32.cplx32.imag, 0,
		//		14 + norm_Q - lo_rhhstar_exp + FIXED_PT_RIGHT_SHIFT);
		shr = 14 + norm_Q + FIXED_PT_RIGHT_SHIFT - lo_rhhstar_exp; 
		lo_yhatC32.cplx32.imag = lo_yhatC32.cplx32.imag >> shr;


		_amem8(&yhstarArray[i]) = lo_yhstarC32.realimag;
		//_amem4(&equalizedPnts[i]) = _pack2(lo_yhat_i, lo_yhat_q);
		_amem4(&equalizedPnts[i]) = _pack2(lo_yhatC32.cplx32.real, lo_yhatC32.cplx32.imag);

	}
//	LOG_PRINTF("norm_min: %d", norm_min);
	*yhstar_norm_min = lo_norm_min;
}

