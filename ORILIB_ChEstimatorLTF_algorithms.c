/**
Atomix project, ORILIB_ChEstimatorLTF_algorithms.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <c6x.h>

#include <src/DSP_recip16/c66/DSP_recip16.h>
#include <src/DSP_fft16x16/c66/DSP_fft16x16.h>
#include <src/DSP_fft16x16/c66/gen_twiddle_fft16x16.h>

#include "ORILIB_ChEstimatorLTF_t.h"
#include "WIFILIB_util.c"

void ORILIB_ChEstimatorLTF_internal_ltfPilotSignNormalization(
	Cplx16 const * const restrict reference_pilot_sequence,
	Cplx16 const * const restrict ltf_sequence,
	Cplx16 * const restrict ltf_gains		//sign-normalized ltf sequence
	){
	
	Uint32 i;
#pragma MUST_ITERATE(52, 52, 52);
	for (i = 0; i < 52; i++) {
		_amem4((void *)&ltf_gains[i]) = _dapys2(
				_amem4((void *)&reference_pilot_sequence[i]),
				_amem4((void *)&ltf_sequence[i]));
	}
}


void ORILIB_ChEstimatorLTF_internal_estimateFromLtf(
	IN  Cplx16 	tOfdmBlock[restrict],
	OUT Cplx16 	subCarrierGains[restrict],
	IN  int16_t const * const restrict twiddle_factors_w64
	) {

	ASSERT_PTR_ALIGNED(twiddle_factors_w64, 8);
	ASSERT_PTR_ALIGNED(tOfdmBlock, 8);
	ASSERT_PTR_ALIGNED(subCarrierGains, 8);

	//----------------- FFT -----------------
	/* TODO: not sure how DSP_fft treats the input and guard against overflow,
	 * since fft will add up 64 quantities (appropriately multiplied by factors).
	 * With some agc room in input scaling, usually, overflow should not occur,
	 * so I'm not doing anything special here. --Manu
	 */
	DSP_fft16x16(  twiddle_factors_w64,
                     PHY_OFDM_N_FFT,
                     (Int16 *) tOfdmBlock,
		     (Int16 *)subCarrierGains
		     );
}

//------------------------------------------------------------------------------------

Int16 gnorm_hhstar[52];

#define CONSTELLATION_MAGNIFY_FACTOR  1.5


void ORILIB_ChEstimatorLTF_internal_channelGainsAvgShiftSignfix(
	IN  Cplx16 const * const restrict channelGains1raw, 	
	IN  Cplx16 const * const restrict channelGains2raw,	
	IN  Cplx16 const * const restrict reference_pilot_sequence,
	OUT Cplx16 * const restrict avgChannelGains		
	){

	Uint64 cg1, cg2;
	Uint32 i;

	Uint64 buf1[32];
	Uint64 buf2[26];

	ASSERT_PTR_ALIGNED(channelGains1raw, 8);
	ASSERT_PTR_ALIGNED(channelGains2raw, 8);
	ASSERT_PTR_ALIGNED(avgChannelGains, 8);

	Cplx16 * aFFftScratchpad = (Cplx16 *)buf1;
	Cplx16 * FFftRearrange = (Cplx16 *)buf2;

	//----------------- AVERAGE -----------------
#pragma MUST_ITERATE(64/2,64/2,64/2);	
	for (i = 0; i < 64; i+=2) {
		cg1 = _amem8((void *)&channelGains1raw[i]);
		cg2 = _amem8((void *)&channelGains2raw[i]);
		_amem8((void *)&aFFftScratchpad[i]) = _davg2(cg1, cg2);
	}


	//----------------- FFT SHIFT -----------------
	WIFILIB_fftShift64(aFFftScratchpad, FFftRearrange);

	//----------------- PILOT SIGN NORMALIZATION -----------------
#pragma MUST_ITERATE(52, 52, 52);
	for (i = 0; i < 52; i++) {
		_amem4((void *)&avgChannelGains[i]) = _dapys2(
				_amem4((void *)&reference_pilot_sequence[i]),
				_amem4((void *)&FFftRearrange[i]));
	}
}




//all arrays are 52 long
void ORILIB_ChEstimatorLTF_internal_channelInverse(
			IN  Cplx16 const * const restrict avgChannelGains,
			OUT Cplx16 * const restrict avgChannelGainsConj,
			OUT Uint32 * const restrict hhstar,
			OUT Uint16 * const restrict recip_hhstar_frac,
			OUT Int16 * const restrict recip_hhstar_exp
			) { 
	Uint32 i = 0;
	

	Uint32 recip_hhstar_exp_tmq_mem[26];
	Uint16 * recip_hhstar_exp_tmq = (Uint16 *)recip_hhstar_exp_tmq_mem;

	Uint32 recip_hhstar_exp_tmp_mem[26];
	Uint16 * recip_hhstar_exp_tmp = (Uint16 *)recip_hhstar_exp_tmp_mem;


	ASSERT_PTR_ALIGNED(avgChannelGains, 8);
	ASSERT_PTR_ALIGNED(avgChannelGainsConj, 8);
	ASSERT_PTR_ALIGNED(hhstar, 8);
	ASSERT_PTR_ALIGNED(recip_hhstar_exp, 8);
	ASSERT_PTR_ALIGNED(recip_hhstar_frac, 8);

#pragma MUST_ITERATE(52/2,52/2,52/2);
	for (i = 0; i < 52; i+=2) {
		Uint64 h, H; //hConj
		Uint32 hhs1, hhs2; //hhs; //hhstarval;
		Uint64 hhs;
		Uint32 exp, nhhs; //normalized_hhstarval;
		Uint16 exp1, exp2, nhhs1, nhhs2;

		h = _amem8((void *)&avgChannelGains[i]);
		
		H = _dapys2(0x0001FFFF0001FFFF, h);
		
		//hhs = _dotp2(h, h);
		//Becomes Q30 and is always positive
		//Also, S1I1Q30 always has I bit 0
		
		//find scale and normalize number into 16 bits
		//exp = _norm(hhs);
		//nhhs = (Uint16)_ext(hhs, exp, 16);		
		//this is a floating representation
		//of the value of hhstar where h was
		//interpreted as being a number expressed
		//as Q15 input. normalized_hhstarval is a
		//Q15 representation of the mantissa part
		//such that hhstar value is 
		//normalized_hhstarval(Q15) * 2^(-exp)

		hhs1 = _dotp2(_hill(h), _hill(h));
		exp1 = _norm(hhs1);
		nhhs1 = (Uint16)_ext(hhs1, exp1, 16);		

		hhs2 = _dotp2(_loll(h), _loll(h));
		exp2 = _norm(hhs2);
		nhhs2 = (Uint16)_ext(hhs2, exp2, 16);		

		hhs = _itoll(hhs1, hhs2);
		nhhs = _pack2(nhhs1, nhhs2);
		
		//this is to make the exponent go up 
		//so that the mantissa can be multiplied
		//by 0.75 (factor less than one) and get
		//an effective multiplication by 1.50.
#ifdef CONSTELLATION_MAGNIFY_FACTOR
		//exp += 1;
		exp = _pack2(exp1, exp2);
		exp = _add2(exp, 0x00010001);
#endif


		//assign to output arrays
		_amem8(&avgChannelGainsConj[i]) = H;
		_amem8(&hhstar[i]) = hhs;	//Q30
		_amem4(&recip_hhstar_exp_tmp[i]) = exp;
		_amem4(&gnorm_hhstar[i]) = nhhs;
	}
	
	//reciprocate array
	DSP_recip16(
		gnorm_hhstar, 
		(Int16 *)recip_hhstar_frac, 
		(Int16 *)recip_hhstar_exp_tmq,
		52
		);
			//At this point, the value reported by 
			//this function is the true reciprocal of
			//normalized_hhstarval, which is 
			//recip_hhstar_frac(Q15) * 2^recip_hhstar_exp.

			//The true reciprocal value of hhstarval is
			//actually recip_hhstar_frac(Q15) *
			//2^(recip_hhstar_exp + recip_hhstar_exp_tmp),
			//so we'll add up the exponents.
																		
//loop to scale up recip_hhstar by a factor of 1.5 (for constellation magnification)
#ifdef CONSTELLATION_MAGNIFY_FACTOR

#pragma MUST_ITERATE(52/4,52/4,52/4);
	for (i = 0; i < 52; i+=4) {
	  Uint64 rf, rfby2;
	  rf = _amem8(&recip_hhstar_frac[i]);
	  rf = _dshr2(rf, 1);
	  rfby2 = _dshr2(rf, 1);
	  rf = _dadd2(rf, rfby2);
	  _amem8(&recip_hhstar_frac[i]) = rf;
	}
#endif
        //(at this point, rf = original value * 0.75)
        //to get the effect of multiply by 1.5, we'll touch the exponent.
        //to save cycles, this touching is being added above in tmp.


	//add up exponents
	{
	  Int32 * restrict ptr1 = (Int32 *)recip_hhstar_exp_tmq;
	  Int32 * restrict ptr2 = (Int32 *)recip_hhstar_exp_tmp;
#pragma MUST_ITERATE(26/2, 26/2, 26/2);
	for (i = 0; i < 26; i+=2)
	  _amem8(&((Uint32 *)recip_hhstar_exp)[i]) = 
	  	_dadd2(_amem8(&ptr1[i]), _amem8(&ptr2[i]));
	}

	//At this point, the true reciprocal value of hhstarval is
	//actually recip_hhstar_frac(Q15) * 2^(recip_hhstar_exp).
	//But since hhstarval itself was Q30 to begin with, the
	//true hhstar reciprocal value, that is, the floating point
	//value, is actually recip_hhstar_frac * 2^(recip_hhstar_exp - 30).

}


/* Returns a floating point value of recip_hhstar where the true value of this
 * reciprocal is recip_hhstar_frac(Q15) * 2^recip_hhstar_exp.
 *
 * Returns a fixed point hhstar which is Q30 if input channel gains are
 * interpreted as Q15.
 *
 * This new version uses the floating point unit.
 */
void ORILIB_ChEstimatorLTF_internal_channelGainsAverage_floating_point(
			IN  Cplx16 channelGains1[restrict], 	//must be 4-byte aligned
			IN  Cplx16 channelGains2[restrict],	//must be 4-byte aligned
			OUT Cplx16 avgChannelGains[restrict],
			OUT Cplx16 avgChannelGainsConj[restrict],
			OUT Uint32 hhstar[restrict],
			OUT float recip_hhstarF[restrict],
			IN  Uint32 size				//must be even
			) {
	Uint32 i = 0;
	Uint32 hhstarval;

	Uint32 cg1, cg2;
	float hhstarF, rhhstarF;

	Cplx16U h, hConj;

	ASSERT_PTR_ALIGNED(channelGains1, 8);
	ASSERT_PTR_ALIGNED(channelGains2, 8);
	ASSERT_PTR_ALIGNED(avgChannelGains, 8);
	ASSERT_PTR_ALIGNED(avgChannelGainsConj, 8);
	ASSERT_PTR_ALIGNED(hhstar, 8);
	ASSERT_PTR_ALIGNED(recip_hhstarF, 8);

#ifdef DEBUG_MODE
	assert(size == 52);
#endif
	_nassert(size == 52);

#pragma MUST_ITERATE(52,52,52);	
	for (i = 0; i < 52; i++) {
		cg1 = _amem4(&channelGains1[i]);
		cg2 = _amem4(&channelGains2[i]);
		h.realimag = _avg2(cg1, cg2);

		hConj.cplx16.real = h.cplx16.real;
		hConj.cplx16.imag = -h.cplx16.imag; 
		_amem4(&avgChannelGainsConj[i]) = hConj.realimag;

		_amem4(&avgChannelGains[i]) = h.realimag;    //Interpret as Q15
		hhstarval = _dotp2(h.realimag, h.realimag);  
		//Becomes Q30 and is always positive
		//Also, S1I1Q30 always has I bit 0

		hhstar[i] = hhstarval;

		hhstarF = (float)hhstarval + 1.0;  //+1 is to ensure no divide-by-zero occurs
		rhhstarF = _rcpsp(hhstarF);

		//TODO: reciprocal multipler is being magnified so that the
		//equalized constellation points are magnified. However, no
		//other parts of the channel estimate are being magnified by
		//this limited scaling. The reason for this magnification is
		//explained in WIFILIB_lookupTables.c. If the channel estimator
		//is later switched to fixed-point version, scaling should also
		//be included there.
		rhhstarF *= CONSTELLATION_MAGNIFY_FACTOR;

		recip_hhstarF[i] = rhhstarF;
	}

}


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
	){

	ASSERT_PTR_ALIGNED(avgChannelGains, 8);
	ASSERT_PTR_ALIGNED(avgChannelGains_temp, 8);

	_amem4cpy(avgChannelGains,		avgChannelGains_temp	,5);
	_amem4cpy(avgChannelGains + 5,avgChannelGains_temp+6	,13);
	_amem4cpy(avgChannelGains + 18,avgChannelGains_temp+20 ,12);
	_amem4cpy(avgChannelGains + 30,avgChannelGains_temp+33 ,13);
	_amem4cpy(avgChannelGains + 43,avgChannelGains_temp+47	,5);
	
	ASSERT_PTR_ALIGNED(avgChannelGainsConj, 8);
	ASSERT_PTR_ALIGNED(avgChannelGainsConj_temp, 8);

	_amem4cpy(avgChannelGainsConj,avgChannelGainsConj_temp,5);
	_amem4cpy(avgChannelGainsConj + 5,avgChannelGainsConj_temp+6,13);
	_amem4cpy(avgChannelGainsConj + 18,avgChannelGainsConj_temp+20,12);
	_amem4cpy(avgChannelGainsConj + 30,avgChannelGainsConj_temp+33,13);
	_amem4cpy(avgChannelGainsConj + 43,avgChannelGainsConj_temp+47,5);
	
	ASSERT_PTR_ALIGNED(hhstar, 8);
	ASSERT_PTR_ALIGNED(hhstar_temp, 8);

	_amem4cpy(hhstar,hhstar_temp,5);
	_amem4cpy(hhstar + 5,hhstar_temp+6,13);
	_amem4cpy(hhstar + 18,hhstar_temp+20,12);
	_amem4cpy(hhstar + 30,hhstar_temp+33,13);
	_amem4cpy(hhstar + 43,hhstar_temp+47,5);

	memcpy(recip_hhstar_frac,recip_hhstar_frac_temp,5*sizeof(Uint16));
	memcpy(recip_hhstar_frac + 5,recip_hhstar_frac_temp+6,13*sizeof(Uint16));
	memcpy(recip_hhstar_frac + 18,recip_hhstar_frac_temp+20,12*sizeof(Uint16));
	memcpy(recip_hhstar_frac + 30,recip_hhstar_frac_temp+33,13*sizeof(Uint16));
	memcpy(recip_hhstar_frac + 43,recip_hhstar_frac_temp+47,5*sizeof(Uint16));

	memcpy(recip_hhstar_exp,recip_hhstar_exp_temp,5*sizeof(Uint16));
	memcpy(recip_hhstar_exp + 5,recip_hhstar_exp_temp+6,13*sizeof(Uint16));
	memcpy(recip_hhstar_exp + 18,recip_hhstar_exp_temp+20,12*sizeof(Uint16));
	memcpy(recip_hhstar_exp + 30,recip_hhstar_exp_temp+33,13*sizeof(Uint16));
	memcpy(recip_hhstar_exp + 43,recip_hhstar_exp_temp+47,5*sizeof(Uint16));

	/*--------------- Save the pilots separately. ---------------*/
	ASSERT_PTR_ALIGNED(avgChannelGains_Pilots, 4);
	ASSERT_PTR_ALIGNED(avgChannelGainsConj_Pilots, 4);
	ASSERT_PTR_ALIGNED(hhstar_Pilots, 4);

	_amem4(&avgChannelGains_Pilots[0]) = _amem4(&avgChannelGains_temp[5]);
	_amem4(&avgChannelGains_Pilots[1]) = _amem4(&avgChannelGains_temp[19]);
	_amem4(&avgChannelGains_Pilots[2]) = _amem4(&avgChannelGains_temp[32]);
	_amem4(&avgChannelGains_Pilots[3]) = _amem4(&avgChannelGains_temp[46]);


	_amem4(&avgChannelGainsConj_Pilots[0]) = _amem4(&avgChannelGainsConj_temp[5]);
	_amem4(&avgChannelGainsConj_Pilots[1]) = _amem4(&avgChannelGainsConj_temp[19]);
	_amem4(&avgChannelGainsConj_Pilots[2]) = _amem4(&avgChannelGainsConj_temp[32]);
	_amem4(&avgChannelGainsConj_Pilots[3]) = _amem4(&avgChannelGainsConj_temp[46]);

	_amem4(&hhstar_Pilots[0]) = _amem4(&hhstar_temp[5]);
	_amem4(&hhstar_Pilots[1]) = _amem4(&hhstar_temp[19]);
	_amem4(&hhstar_Pilots[2]) = _amem4(&hhstar_temp[32]);
	_amem4(&hhstar_Pilots[3]) = _amem4(&hhstar_temp[46]);

	recip_hhstar_frac_Pilots[0] = recip_hhstar_frac_temp[5];
	recip_hhstar_frac_Pilots[1] = recip_hhstar_frac_temp[19];
	recip_hhstar_frac_Pilots[2] = recip_hhstar_frac_temp[32];
	recip_hhstar_frac_Pilots[3] = recip_hhstar_frac_temp[46];

	recip_hhstar_exp_Pilots[0] = recip_hhstar_exp_temp[5];
	recip_hhstar_exp_Pilots[1] = recip_hhstar_exp_temp[19];
	recip_hhstar_exp_Pilots[2] = recip_hhstar_exp_temp[32];
	recip_hhstar_exp_Pilots[3] = recip_hhstar_exp_temp[46];
}
