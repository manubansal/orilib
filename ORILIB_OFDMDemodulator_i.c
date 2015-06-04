/**
Atomix project, ORILIB_OFDMDemodulator_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>

#include <src/DSP_fft16x16/c66/DSP_fft16x16.h>

#include "ORILIB_OFDMDemodulator_t.h"
#include "ORILIB_utils_inl.h"

//#include <util/TEST_printDebug.h>

	
//	DEBUG(
//	static int count = 0;
//	/****** symbol after channel equalization, including pilot phase removal ******/
//	LOG_PRINTF("ofdm demodulator count = %d\n", count++);
//	LOG_PRINTF("input samples\n");
//	printCplx16SingleColumn((Cplx16 *)ofdm_symbol.rwPointer, 80);
//	LOG_PRINTF("data subcarriers\n");
//	printCplx16SingleColumn((Cplx16 *)data_subcarriers.rwPointer, 48);
//	LOG_PRINTF("pilot subcarriers\n");
//	printCplx16SingleColumn((Cplx16 *)pilot_subcarriers.rwPointer, 4);
//	)

#pragma DATA_SECTION(tempFftOutput, ".data:tmp");
#pragma DATA_SECTION(tOfdmBlockCopy, ".data:tmp");
static	Cplx16 tempFftOutput[64];
static	Cplx16 tOfdmBlockCopy[PHY_OFDM_N_FFT]; //this is used to copy in from tOfdmBlock before

//IN 	ComplexSamples const * const restrict ofdm_time_symbol, 
//OUT	ComplexSamples * const restrict data_subcarriers,
//OUT 	ComplexSamples * const restrict pilot_subcarriers,
//INOUT	OfdmDemodulator64p16cp_state * const state
void ORILIB_OFDMDemodulator_i ( 
	IN 	ORILIB_t_Cplx16Buf80 		* inpSampleBuf,
	IN	ORILIB_OFDMDemodulator_t_State 	* inpStateBuf,
	OUT	ORILIB_t_Cplx16Buf48 		* outDataSubcarriersBuf,
	OUT	ORILIB_t_Cplx16Buf4  		* outPilotSubcarriersBuf,
	OUT	ORILIB_OFDMDemodulator_t_State 	* outStateBuf
	) {
	
	//////////////////////////////Cplx16 tempFftOutput[64];
	Cplx16 *ofdm_time_symbol = inpSampleBuf->samples;
	Cplx16 *data_subcarriers = outDataSubcarriersBuf->samples;
	Cplx16 *pilot_subcarriers = outPilotSubcarriersBuf->samples;


	/** Get the pilot symbol, which is either positive or negative of the scale, depending upon the pilot polarity */
	int32_t pilot_polarity_seq_counter = inpStateBuf->pilot_polarity_counter;
//	int16_t pilot_symbol =
	uint32_t whether_to_flip =		//int64 because the four 16-bit parts will reflect the same sign as the pilot symbol upon this assignment;
			 //1 will give 0 for all sign bits, -1 will giuve 1 for all sign bits.
//	 	( (state->pilot_polarity_sequence[pilot_polarity_seq_counter] == 1) ? scale_factor : -scale_factor);
	//( (state->pilot_polarity_sequence[pilot_polarity_seq_counter]) == -1);
	( (ORILIB_pilot_polarity_sequence[pilot_polarity_seq_counter]) == -1);

	 //what is not thine, thou shalt not touch, says i.
	////////////////////////////Cplx16 tOfdmBlockCopy[PHY_OFDM_N_FFT]; //this is used to copy in from tOfdmBlock before
	 //passing it to FFT because DSP_fft function uses
	 //this input buffer as a scratchpad and modifies it.
	 //Most obnoxious.

//	 memcpy(
//			 tOfdmBlockCopy,
//			 ofdm_time_symbol + CYCLIC_PREFIX_LENGTH_TO_DROP,
//			 PHY_OFDM_N_FFT * sizeof(Cplx16)
//	 );
	uint32_t i;
#pragma MUST_ITERATE(PHY_OFDM_N_FFT/2,PHY_OFDM_N_FFT/2,PHY_OFDM_N_FFT/2);
	for(i=0; i < (PHY_OFDM_N_FFT); i+=2)
	{
		_amem8(tOfdmBlockCopy + i) = 
			_amem8_const(ofdm_time_symbol + CYCLIC_PREFIX_LENGTH_TO_DROP + i);
	}
	

	/** Remove the cyclic prefix and take the FFT */
	 //note: this scales up by a factor of 16
	DSP_fft16x16(
		 //state->twiddle_factors,
		 ORILIB_twiddle_factors_fft16x16_64,
                 64,
                 (int16_t *)(tOfdmBlockCopy),
                 (int16_t *)tempFftOutput);
	 
	/** Write out the pilot symbols in normal order.
	 * Use _amem4 r/w here, because ComplexSamples are 4 bytes each */
//	_amem4(&pilot_subcarriers[0]) = _amem4(&tempFftOutput[43]);
//	_amem4(&pilot_subcarriers[1]) = _amem4(&tempFftOutput[57]);
//	_amem4(&pilot_subcarriers[2]) = _amem4(&tempFftOutput[7]);
//	_amem4(&pilot_subcarriers[3]) = _amem4(&tempFftOutput[21]);

	//normalize pilot points so that they are all h.1, that is, are only
	//scaled by the complex channel, but pilot polarities have been normalized to 1
	Cplx16U pilot0; pilot0.realimag = _amem4(&tempFftOutput[43]);
	Cplx16U pilot1; pilot1.realimag = _amem4(&tempFftOutput[57]);
	Cplx16U pilot2; pilot2.realimag = _amem4(&tempFftOutput[7]);
	Cplx16U pilot3; pilot3.realimag = _amem4(&tempFftOutput[21]);


	pilot0.cplx16.real = whether_to_flip ? -pilot0.cplx16.real : pilot0.cplx16.real;
	pilot0.cplx16.imag = whether_to_flip ? -pilot0.cplx16.imag : pilot0.cplx16.imag;
	pilot1.cplx16.real = whether_to_flip ? -pilot1.cplx16.real : pilot1.cplx16.real;
	pilot1.cplx16.imag = whether_to_flip ? -pilot1.cplx16.imag : pilot1.cplx16.imag;
	pilot2.cplx16.real = whether_to_flip ? -pilot2.cplx16.real : pilot2.cplx16.real;
	pilot2.cplx16.imag = whether_to_flip ? -pilot2.cplx16.imag : pilot2.cplx16.imag;
	pilot3.cplx16.real = whether_to_flip ? pilot3.cplx16.real : -pilot3.cplx16.real;
	pilot3.cplx16.imag = whether_to_flip ? pilot3.cplx16.imag : -pilot3.cplx16.imag;

	_amem4(&pilot_subcarriers[0]) = pilot0.realimag;
	_amem4(&pilot_subcarriers[1]) = pilot1.realimag;
	_amem4(&pilot_subcarriers[2]) = pilot2.realimag;
	_amem4(&pilot_subcarriers[3]) = pilot3.realimag;

	//state->pilot_polarity_counter = (pilot_polarity_seq_counter + 1)%127;
	outStateBuf->pilot_polarity_counter = (pilot_polarity_seq_counter + 1)%127;

	
	/** Write out the data subcarriers, in normal order. */
		/* Copy -24 to -1 data subcarriers to output */
//	memcpy(&data_subcarriers[0], &tempFftOutput[38], 5 * sizeof(Cplx16));
//	memcpy(&data_subcarriers[5], &tempFftOutput[44], 13 * sizeof(Cplx16));
//	memcpy(&data_subcarriers[18], &tempFftOutput[58], 6 * sizeof(Cplx16));  
//	
//	/* Copy 0 to 23 data subcarriers to output */
//	memcpy(&data_subcarriers[24], &tempFftOutput[1], 6 * sizeof(Cplx16));
//	memcpy(&data_subcarriers[30], &tempFftOutput[8], 13 * sizeof(Cplx16));
//	memcpy(&data_subcarriers[43], &tempFftOutput[22], 5 * sizeof(Cplx16));

#pragma MUST_ITERATE(5,5,5)
	for(i=0; i < 5; i++)
	{
		_amem4(data_subcarriers + 0 + i) = 
			_amem4(tempFftOutput + 38 + i);
	}
#pragma MUST_ITERATE(13,13,13)
	for(i=0; i < 13; i++)
	{
		_amem4(data_subcarriers + 5 + i) = 
			_amem4(tempFftOutput + 44 + i);
	}
#pragma MUST_ITERATE(6,6,6)
	for(i=0; i < 6; i++)
	{
		_amem4(data_subcarriers + 18 + i) = 
			_amem4(tempFftOutput + 58 + i);
	}

#pragma MUST_ITERATE(6,6,6)
	for(i=0; i < 6; i++)
	{
		_amem4(data_subcarriers + 24 + i) = 
			_amem4(tempFftOutput + 1 + i);
	}
	
#pragma MUST_ITERATE(13,13,13)
	for(i=0; i < 13; i++)
	{
		_amem4(data_subcarriers + 30 + i) = 
			_amem4(tempFftOutput + 8 + i);
	}
#pragma MUST_ITERATE(5,5,5)
	for(i=0; i < 5; i++)
	{
		_amem4(data_subcarriers + 43 + i) = 
			_amem4(tempFftOutput + 22 + i);
	}


	//LOG_PRINTF("whether_to_flip: %d", whether_to_flip);

	DEBUG_DATA (
	LOG_PRINTF("data subcarriers before equalization but after ofdm demod:\n");
	printCplx16SingleColumn(outDataSubcarriersBuf->samples, 48);

	LOG_PRINTF("sign-normalized pilot subcarriers before equalization but after ofdm demod:\n");
	printCplx16SingleColumn(outPilotSubcarriersBuf->samples, 4);
	)

}
