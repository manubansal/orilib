/**
Atomix project, ORILIB_CFOEstimatorLTF_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_t.h"
#include "ORILIB_CFOEstimatorLTF_t.h"

#include "ORILIB_utils_inl.h"
#include "WIFILIB_lookupTables.h"
//#include "IQmath_inline_all.h"
#include "IQmath_inline.h"

#include <c6x.h>

//Uint32 *sign_cfo,			//1 if negative, 0 otherwise
void ORILIB_CFOEstimatorLTF_i (
		IN	ORILIB_t_Cplx16Buf160 const * const inpFullLtfBuf,
		IN	ORILIB_t_PktAgcState const * const restrict agcState,
		OUT	ORILIB_t_CFOState * const restrict cfoState
		) {

		//OUT	Uint32 * const restrict phiU,
		//OUT	Uint32 * const restrict sign_cfo,
		//OUT	Uint32 * const restrict derot_table_stride,
		//OUT	Cplx16U const ** const restrict derotTablePtr

	Uint32 i;
	Uint32 lo_phiU;
	Uint32 sign_y;
	Uint32 lo_derot_table_stride;

	Uint32 pktAgcGain;

	Int32 corrTermShiftAmount;
	Cplx32U lo_currWinCorr;
	Cplx32U d;

    {
	Cplx16 const * const restrict ltf1 = &inpFullLtfBuf->samples[SYNC_LTF_CP_LENGTH];
	Cplx16 const * const restrict ltf2 = &inpFullLtfBuf->samples[SYNC_LTF_CP_LENGTH + SYNC_LTF_PERIOD];

	pktAgcGain = agcState->agcGain;


	//example 1: if the agc determines it is safe to have a gain of 4 bits, and the
	//ltf period length is 6 bits (64), we are safe, since the sample occupies 15-4=11 bits,
	//so the product of a*a occupies 22 bits, which has a room of 9 bits to expand, which
	//is more than the 6-bit room we need for a 64-sized window sum. the additional safety
	//margin ensures that no overflow/saturation occurs even when agc gain threshold is picked
	//to be aggressive for better peak detection performance.

	//example 2: suppose agc gain value is 2 bits. then, most extreme samples occupy 15 - 2 = 13 bits,
	//so that product occupies 26 bits, so we only have a room of 5 bits to expand but we need 6,
	//so we should shift right by at least 1 bit, plus the further-safety-margin. our corrTermShiftAmount
	//returns 6 - (2 * 2 + 1) + safety-margin = (1 + safety-margin) when the ltf period is 64, and
	//we retain that value since it's positive.


#if (SYNC_ALIGNED_SAMPLES_WITH_GAIN == 1)
	corrTermShiftAmount = SYNC_LTF_PERIOD_N_BITS - (2 * 0) +
				SYNC_AGC_FURTHER_SAFETY_MARGIN_FOR_ABSOLUTELY_NO_SATURATION;
#else
	corrTermShiftAmount = SYNC_LTF_PERIOD_N_BITS - (2 * pktAgcGain) +
					SYNC_AGC_FURTHER_SAFETY_MARGIN_FOR_ABSOLUTELY_NO_SATURATION;
#endif
	corrTermShiftAmount = corrTermShiftAmount > 0 ? corrTermShiftAmount : 0;
	//removed the + 1 so that the maximum magnitude of the sum is 30-bits, not 31-bits.
	//this is because we want to use Q30 arctan later.

	//unoptimized loop, easier to understand the algorithm
//	lo_currWinCorr.realimag = 0;
//#pragma MUST_ITERATE(SYNC_LTF_PERIOD,SYNC_LTF_PERIOD,SYNC_LTF_PERIOD);
//	for (i = 0; i < SYNC_LTF_PERIOD; i++) {
//		Cplx16U currSample, currSampleConj;
//		Cplx16U lo_zwinSample;
//
//		Cplx32U lo_currCorrTermUnscaled;
//		Cplx32U lo_currCorrTermScaled;
//
//
//		//---------------------- load the current sample ----------------------
//
//		currSample.realimag = _amem4(&ltf1[i]);		//Q15 (16bit signed samples in (-1.0,1.0)
//
//		//load up the previous-window shadowing sample (just before current window)
//		lo_zwinSample.realimag = _amem4(&ltf2[i]);
//
//		//conjugate of the current sample being processed
//		currSampleConj.cplx16.real = currSample.cplx16.real;
//		currSampleConj.cplx16.imag = -currSample.cplx16.imag;			//conj
//
//		//---------------------- begin correlation computation ----------------------
//		lo_currCorrTermUnscaled.realimag = _cmpy(currSampleConj.realimag, lo_zwinSample.realimag);
//
//		lo_currCorrTermScaled.cplx32.real = _abs(lo_currCorrTermUnscaled.cplx32.real);
//		lo_currCorrTermScaled.cplx32.imag = _abs(lo_currCorrTermUnscaled.cplx32.imag);
//
//		lo_currCorrTermScaled.cplx32.real >>= corrTermShiftAmount;
//		lo_currCorrTermScaled.cplx32.imag >>= corrTermShiftAmount;
//
//		//restore the sign after bit shift
//		lo_currCorrTermScaled.cplx32.real = lo_currCorrTermUnscaled.cplx32.real < 0 ?
//			-lo_currCorrTermScaled.cplx32.real : lo_currCorrTermScaled.cplx32.real;
//		lo_currCorrTermScaled.cplx32.imag = lo_currCorrTermUnscaled.cplx32.imag < 0 ?
//			-lo_currCorrTermScaled.cplx32.imag : lo_currCorrTermScaled.cplx32.imag;
//
//		lo_currWinCorr.realimag = _dadd(lo_currWinCorr.realimag, lo_currCorrTermScaled.realimag);
//
//		//---------------------- done correlation computation ----------------------
//
//
//	}

	//optimized loop
	d.realimag = 0;
#pragma MUST_ITERATE(SYNC_LTF_PERIOD/4,SYNC_LTF_PERIOD/4,SYNC_LTF_PERIOD/4);
	for (i = 0; i < SYNC_LTF_PERIOD; i+=4) {
		Uint64 x0x1, x2x3, y0y1, y2y3;
		__x128_t x0x1x2x3, y0y1y2y3, c0c1, c2c3;
		Uint64 d01, d23;

		x0x1 = _amem8((void *)(&ltf1[i]));
		x2x3 = _amem8((void *)(&ltf1[i+2]));
		y0y1 = _amem8((void *)(&ltf2[i]));
		y2y3 = _amem8((void *)(&ltf2[i+2]));

		x0x1x2x3 = _llto128(x0x1, x2x3);
		y0y1y2y3 = _llto128(y0y1, y2y3);


		c0c1 = _dccmpy(_hi128(y0y1y2y3), _hi128(x0x1x2x3));	//128bit output
		c2c3 = _dccmpy(_lo128(y0y1y2y3), _lo128(x0x1x2x3));	//128bit output
		d01 = _dadd(_hi128(c0c1),_lo128(c0c1));
		d23 = _dadd(_hi128(c2c3),_lo128(c2c3));
		d01 = _dshr(d01, corrTermShiftAmount);
		d23 = _dshr(d23, corrTermShiftAmount);
		d.realimag = _dadd(d01, d.realimag);
		d.realimag = _dadd(d23, d.realimag);
		//---------------------- done correlation computation ----------------------


	}


	lo_currWinCorr.realimag = d.realimag;

	DEBUG_VERBOSE(LOG_PRINTF("CFOEstimtaor: original correlation complex number: real: %d  imag: %d \n",
			lo_currWinCorr.cplx32.real,
			lo_currWinCorr.cplx32.imag);
	)


	sign_y = lo_currWinCorr.cplx32.imag < 0;	//1 if negative, 0 if non-negative
	lo_currWinCorr.cplx32.imag = sign_y ? -lo_currWinCorr.cplx32.imag : lo_currWinCorr.cplx32.imag;
	//we want to keep the imaginary part positive,
	//so that the angle estimate is in [0,pi), or
	//unit angle estimate is in [0, 0.5). that is
	//all we can determine - other multiples cannot
	//be determined with this algorithm. sign_y also
	//gives the sign of cfo - if sign_y == 1, cfo is
	//negative, else positive.


	DEBUG_VERBOSE(LOG_PRINTF("CFOEstimtaor: running arctan on complex number  : real: %d  imag: %d \n",
			lo_currWinCorr.cplx32.real,
			lo_currWinCorr.cplx32.imag);
	)


//	lo_phiU = _IQ15atan2PU(lo_currWinCorr.cplx32.imag, lo_currWinCorr.cplx32.real);	//lo_phiU is in [0,1) ~ [0,2pi)
	lo_phiU = _IQ30atan2PU(lo_currWinCorr.cplx32.imag, lo_currWinCorr.cplx32.real);	//lo_phiU is in [0,1) ~ [0,2pi)


//	DEBUG(LOG_PRINTF("Q15 lo_phiU after atan: %d  \n", lo_phiU);)
	DEBUG_DATA(LOG_PRINTF("CFOEstimtaor: Q30 lo_phiU after atan: %d  \n", lo_phiU);)

	//this is the angle phi that the sample rotated by over WINDOW_SIZE
	//amount that we rotate per sample, therefore, is phi/WINDOW_SIZE
	//the best we can do is to approximate it by the closest value in our table of size DEROT_TABLE_SIZE
	//thus, we will multiply it by DEROT_TABLE_SIZE and round it. that will tell us the index in the table
	//corresponding to this offset. once we have that, it become our step factor in that table for derotation.

//	lo_phiU = _IQ15div(lo_phiU, SYNC_LTF_PERIOD_Q15);	//phase offset per sample, Q15
//	lo_phiU = _IQ30div(lo_phiU, SYNC_LTF_PERIOD_Q30);	//phase offset per sample, Q15

//	lo_phiU >>= 15;	//makes it Q15
//	lo_phiU = _IQ15div(lo_phiU, SYNC_LTF_PERIOD_Q15);	//phase offset per sample, Q15

	lo_phiU = lo_phiU >> SYNC_LTF_PERIOD_N_BITS;	//Q30 postiive unit phase offset per sample
	DEBUG_DATA(LOG_PRINTF("CFOEstimtaor: Q30 lo_phiU per sample: %d  \n", lo_phiU);)

//	DEBUG(LOG_PRINTF("Q15 lo_phiU after div : %d  \n", lo_phiU);)
//	DEBUG(LOG_PRINTF("Q30 lo_phiU after div : %d  \n", lo_phiU);)

	//	LOG_PRINTF("angle on scale of 32:
	//	lo_phiU = lo_phiU * 1024;	//this becomes Q25
	//since the table size is 1024, our lo_phiU, as an index into the table, becomes Q5

//	lo_phiU >>= (15 - DEROT_TABLE_SIZE_N_BITS);	//this gives lo_phiU as index into derotation table of size 2^DEROT_TABLE_SIZE_N_BITS
//	lo_phiU >>= (30 - DEROT_TABLE_SIZE_N_BITS);	//this gives lo_phiU as index into derotation table of size 2^DEROT_TABLE_SIZE_N_BITS
//	lo_derot_table_stride = lo_phiU >> (30 - DEROT_TABLE_SIZE_N_BITS);
	lo_derot_table_stride = lo_phiU >> (30 - DEROT_TABLE_FULL_SIZE_N_BITS);

//	DEBUG(LOG_PRINTF("Q5 lo_phiU after shift : %d  \n", lo_phiU);)


	////if the cfo is negative, we want to successively multiply with
	////cos th, sin th, so we will use the plusRot table. otherwise, we'll
	////use the minusRot table to multiply with cos th, -sin th.
	//*derotTablePtr = sign_y ? wifilib_plusRot : wifilib_minusRot;

	cfoState->phiU = lo_phiU;
	cfoState->sign_cfo = sign_y;
	cfoState->derot_table_stride = lo_derot_table_stride;

	//if the cfo is negative, we want to successively multiply with
	//cos th, sin th, so we will use the plusRot table. otherwise, we'll
	//use the minusRot table to multiply with cos th, -sin th.
	cfoState->derotTablePtr = sign_y ? wifilib_plusRot : wifilib_minusRot;
    }

}
