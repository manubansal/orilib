/**
Atomix project, ORILIB_WindowCorrelator_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <c6x.h>

#if SYNC_BUFFER_SIZE_ENERGY != 80
#error: Need SYNC_BUFFER_SIZE_ENERGY to be 80
#endif


static void inner(
		IN 	Cplx16 const * const restrict freshSampleBuf_withGain,
		IN	Uint32 windowEnergyE1andE2Buf[restrict],			
		INOUT	Uint32 *positionNextE2Value,
		INOUT	Cplx16	zwinSampleBuf[restrict],
		INOUT	Cplx16	zwinCorrTermBuf[restrict],
		INOUT	Uint32 	*zwinPosition,						
		INOUT	Cplx16	*currWinCorr,
		OUT	Cplx16	winCorrBuf[restrict]
		){


    Uint32 w0; //lo_currWinCorr;
    Uint64 w1w2;
    Uint32 lo_zwinPosition;

    lo_zwinPosition = _amem4(zwinPosition);
    //lo_currWinCorr.realimag = _amem4(currWinCorr);
    w0 = _amem4(currWinCorr);
    w1w2 = _itoll(w0, w0);

    Uint32 i;


    //------------ optimized loop -------------
#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY/2,SYNC_BUFFER_SIZE_ENERGY/2, SYNC_BUFFER_SIZE_ENERGY/2);
    for ( i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i+=2 ) {
	Uint64 x1x2, z1z2; //currSample, lo_zwinSample; 
	Uint64 b1b2, b1b12; //lo_zwinCorrTerm;
	Uint64 c1c2, c1c2_abs, c1c12; //lo_currCorrTerm;
	__x128_t c1c2_32; //lo_currCorrTermCplx32U;
	Cplx32U c1_32, c2_32;

	//load the current sample
	x1x2 = _amem8_const(&freshSampleBuf_withGain[i]);		

	//load the delayed sample
	z1z2 = _amem8(&zwinSampleBuf[lo_zwinPosition]);

	//load the correlation term dropping out of the window
	b1b2 = _amem8(&zwinCorrTermBuf[lo_zwinPosition]);


	//---------------------- begin correlation computation ----------------------
	//multiply conjugate with delayed version
	c1c2_32 = _dccmpy(z1z2, x1x2);
	c1_32.realimag = _hi128(c1c2_32);
	c2_32.realimag = _lo128(c1c2_32);

	//retain 16-bits from absolute values
	c1c2 = _dpackh2(c1_32.realimag, c2_32.realimag);
	c1c2 = _dpack2(_hill(c1c2), _loll(c1c2));

	//shift right to avoid overflow, but shift in absolute value to avoid
	//lower-rounding roundoff error of negative integer right-shift
	c1c2_abs = _dapys2(c1c2, c1c2);
	c1c2_abs = _dshr2(c1c2_abs, (SYNC_WINDOW_SIZE_ENERGY__ACCUMULATION_RSHIFT));
	c1c2 = _dapys2(c1c2, c1c2_abs);

	//update the autocorrelation moving sum (note that these terms are full
	//complex numbers)
	b1b12 = _dadd2(b1b2, _itoll(0, _hill(b1b2)));
	c1c12 = _dadd2(c1c2, _itoll(0, _hill(c1c2)));
	w1w2 = _dsub2(w1w2, b1b12);
	w1w2 = _dadd2(w1w2, c1c12);

	//----------------------
	_amem8(&zwinSampleBuf[lo_zwinPosition]) = x1x2;
	_amem8(&zwinCorrTermBuf[lo_zwinPosition]) = c1c2;
	_amem8(&winCorrBuf[i]) = w1w2;

	w0 = _loll(w1w2);
	w1w2 = _itoll(w0, w0);

	lo_zwinPosition += 2;
	lo_zwinPosition = lo_zwinPosition < SYNC_WINDOW_SIZE_ENERGY ? 
		lo_zwinPosition : (lo_zwinPosition - SYNC_WINDOW_SIZE_ENERGY);
    }
    //------------ optimized loop -------------

    _amem4(zwinPosition) 		= lo_zwinPosition;
    _amem4(currWinCorr) 		= w0;
}

static void inner_unoptimized(
		//-- energy-only related fields --
		IN 	Cplx16 const freshSampleBuf_withGain[restrict],			
		//new buffer of samples, length SYNC_BUFFER_SIZE_ENERGY
		IN	Uint32 windowEnergyE1andE2Buf[restrict],			
		//buffer to hold window energy values, length = SYNC_BUFFER_SIZE_ENERGY

		INOUT	Uint32 *positionNextE2Value,

		//-- correlation computation specific fields --
		INOUT	Cplx16	zwinSampleBuf[restrict],
		INOUT	Cplx16	zwinCorrTermBuf[restrict],
		INOUT	Uint32 	*zwinPosition,						
		//next position of the zwinSampleEnergyTerm buffer to be used
		INOUT	Cplx16	*currWinCorr,

		OUT	Cplx16	winCorrBuf[restrict]
		){

    //-- energy related variables --

    //-- correlation related variables --

    Cplx16U lo_currWinCorr;
    Cplx16U lo_zwinCorrTerm;
    Uint32 lo_zwinPosition;

    lo_zwinPosition = _amem4(zwinPosition);
    lo_currWinCorr.realimag = _amem4(currWinCorr);

    Uint32 i;

    //------------ simpler loop ---------------
#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY,SYNC_BUFFER_SIZE_ENERGY, SYNC_BUFFER_SIZE_ENERGY);
    for ( i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i++ ) {
	Cplx16U currSample, currSampleConj;

	Cplx16U lo_currCorrTerm;
	Cplx32U lo_currCorrTermCplx32U;
	Cplx16U lo_zwinSample;

	Cplx32U sh_cplx32;

	currSample.realimag = _amem4_const(&freshSampleBuf_withGain[i]);		
	currSampleConj.cplx16.real = currSample.cplx16.real;
	currSampleConj.cplx16.imag = -currSample.cplx16.imag;

	//---------------------- begin correlation computation ----------------------
	lo_zwinSample.realimag = _amem4(&zwinSampleBuf[lo_zwinPosition]);

	lo_currCorrTermCplx32U.realimag = _cmpy(currSampleConj.realimag, lo_zwinSample.realimag);

	sh_cplx32.cplx32.real = _abs(lo_currCorrTermCplx32U.cplx32.real);
	sh_cplx32.cplx32.imag = _abs(lo_currCorrTermCplx32U.cplx32.imag);
	lo_currCorrTerm.realimag = _packh2(sh_cplx32.cplx32.real, sh_cplx32.cplx32.imag);

	//this is to avoid overflow
	lo_currCorrTerm.realimag = _shr2(lo_currCorrTerm.realimag, 
		(SYNC_WINDOW_SIZE_ENERGY__ACCUMULATION_RSHIFT));
	//While there some sense to the right-shift above, since that many
	//values are being added to assemble the correlation sum for the
	//average window correlation, on the regression trace, the above
	//statement just leads to total lost of significant bits due to the
	//right shift. --MB 02/07/2014.

	//restore the sign after bit shift
	lo_currCorrTerm.cplx16.real = lo_currCorrTermCplx32U.cplx32.real < 0 ? 
		-lo_currCorrTerm.cplx16.real : lo_currCorrTerm.cplx16.real;
	lo_currCorrTerm.cplx16.imag = lo_currCorrTermCplx32U.cplx32.imag < 0 ? 
		-lo_currCorrTerm.cplx16.imag : lo_currCorrTerm.cplx16.imag;


	//load the correlation term dropping out of the window
	lo_zwinCorrTerm.realimag = _amem4(&zwinCorrTermBuf[lo_zwinPosition]);


	//update the autocorrelation moving sum (note that these terms are full complex numbers)
	lo_currWinCorr.realimag = _sub2(lo_currWinCorr.realimag, lo_zwinCorrTerm.realimag);
	lo_currWinCorr.realimag = _add2(lo_currWinCorr.realimag, lo_currCorrTerm.realimag);

	//---------------------- done correlation computation ----------------------

	_amem4(&zwinSampleBuf[lo_zwinPosition]) = currSample.realimag;
	_amem4(&zwinCorrTermBuf[lo_zwinPosition]) = lo_currCorrTerm.realimag;

	lo_zwinPosition = (lo_zwinPosition + 1) % SYNC_WINDOW_SIZE_ENERGY;

	_amem4(&winCorrBuf[i]) = lo_currWinCorr.realimag;



    }
    //------------ simpler loop ---------------

    _amem4(zwinPosition) 		= lo_zwinPosition;
    _amem4(currWinCorr) 		= _amem4(&lo_currWinCorr);
}



void ORILIB_WindowCorrelator_i(
		IN  ORILIB_t_Cplx16Buf80 const 	* const restrict sampleBufWithGain,
		IN  ORILIB_t_GainStateAux 	* 		inpGainState,
		IN  ORILIB_t_AlignState 	* 		inoutAlignState,
		IN  ORILIB_t_CorrState 		* 		inoutCorrState,
		OUT ORILIB_t_CorrStateAux 	* 		inoutCorrStateAux
		){


	ComplexSamples const * raw_samples_with_gain = sampleBufWithGain->samples;
	ORILIB_t_GainStateAux * gainStateAux = inpGainState;
	ORILIB_t_AlignState * alignState = inoutAlignState;
	ORILIB_t_CorrState * corrState = inoutCorrState;
	ORILIB_t_CorrStateAux * corrStateAux = inoutCorrStateAux;



	Cplx16	* winCorrBuf = corrStateAux->winCorrBuf;

	//compute correlations
	inner(
	  //-- energy-only related fields --
	  IN 	raw_samples_with_gain,			
		  //new buffer of samples, length SYNC_BUFFER_SIZE_ENERGY

	  IN	gainStateAux->windowEnergyE1andE2Buf_withGain,			
		  //buffer to hold window energy values, length = SYNC_BUFFER_SIZE_ENERGY

	  INOUT	&corrState->corr_positionNextE2Value,

	  //-- correlation computation specific fields --
	  INOUT	corrState->corr_zwinSampleBuf,
	  INOUT	corrState->corr_zwinCorrTermBuf,
	  INOUT	&corrState->corr_zwinPosition,						
		  //next position of the zwinSampleEnergyTerm buffer to be used

	  INOUT	&corrState->corr_currWinCorr,

	  OUT	winCorrBuf
	  );
}
