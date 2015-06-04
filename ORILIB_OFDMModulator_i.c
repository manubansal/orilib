/**
Atomix project, ORILIB_OFDMModulator_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <src/DSP_ifft16x16/c66/DSP_ifft16x16.h>

#include "ORILIB_OFDMModulator_t.h"
#include "ORILIB_utils_inl.h"

extern Int16 ORILIB_twiddle_factors_ifft16x16_64[128];

#pragma DATA_SECTION(tempIfftInput, ".data:tmp");
#pragma DATA_ALIGN(tempIfftInput, 8);
static  Cplx16 tempIfftInput[64];

#define CYCLIC_PREFIX_LENGTH 16
#define BYTE_ALIGNMENT 8


/*	default mapping (with DC in the middle) and fftshifted mapping (DC at start):
	* 
	* last column is when leaving first 16 for cyclic prefix (wait, we don't need to 
	* leave space for CP in frequency domain - it's only for time domain. wth.)
	* name|shift|data|temp|
	* d0  | -26 | 6  | 38 | memcpy 5  | 54
	* d4  | -22 | 10 | 42 |
	* p   | -21 | 11 | 43 |           | 59
	* d5  | -20 | 12 | 44 | memcpy 13 | 60
	* d17 | -8  | 24 | 56 |
	* p   | -7  | 25 | 57 |           | 73
	* d18 | -6  | 26 | 58 | memcpy 6  | 74
	* d23 | -1  | 31 | 63 |
	* dc  |  0  | 32 | 0  |           
	* d24 |  1  | 33 | 1  | memcpy 6  | 17
	* d29 |  6  | 38 | 6  |
	* p   |  7  | 39 | 7  |           | 23
	* d30 |  8  | 40 | 8  | memcpy 13 | 24
	* d42 | 20  | 52 | 20 |
	* p   | 21  | 53 | 21 |           | 37
	* d43 | 22  | 54 | 22 | memcpy 5  | 38
	* d47 | 26  | 58 | 26 |
	
	Because this operation modifies the input, and the IFFT algorithm itself
	* destroys the input, we must copy the input data over to a temporary buffer
 */

void ORILIB_OFDMModulator_i ( 
	IN 	ORILIB_t_Cplx16Buf48 		* inpSampleBuf,
	IN	ORILIB_OFDMModulator_t_State 	* inpStateBuf,
	OUT	ORILIB_t_Cplx16Buf80 		* outSampleBuf,
	OUT	ORILIB_OFDMModulator_t_State 	* outStateBuf,
	CF	ORILIB_OFDMModulator_t_Conf	* conf
	) {
	
	Cplx16 * data_subcarriers = inpSampleBuf->samples;
	Cplx16 * ofdm_symbol = outSampleBuf->samples;

	uint32_t in_counter;
	/* First, zero the outermost subcarriers and the dc component, which fall from 27-37 */
	memset(tempIfftInput + 27, 0, 11*sizeof(Cplx16));
	_amem4(tempIfftInput + 0) = 0;
	 
	/* Copy -24 to -1 data subcarriers to temporary input */
	memcpy(&tempIfftInput[38], &data_subcarriers[0], 5 * sizeof(Cplx16));
	memcpy(&tempIfftInput[44], &data_subcarriers[5], 13 * sizeof(Cplx16));
	memcpy(&tempIfftInput[58], &data_subcarriers[18], 6 * sizeof(Cplx16));  

	/* Copy 0 to 23 data subcarriers to temporary input */
	memcpy(&tempIfftInput[1], &data_subcarriers[24], 6 * sizeof(Cplx16));
	memcpy(&tempIfftInput[8], &data_subcarriers[30], 13 * sizeof(Cplx16));
	memcpy(&tempIfftInput[22], &data_subcarriers[43], 5 * sizeof(Cplx16));

	/* fill in pilots */
	/** Get the pilot scale factor (should match the rms scale of the subcarrier constellation mapper */
	int16_t scaleFactor = conf->scaleFactor; // pilot symbols = 1

	/** Get the pilot symbol, which is either positive or negative of the scale, depending upon the pilot polarity */
	int32_t pilot_polarity_seq_counter = inpStateBuf->pilot_polarity_counter;
	int16_t pilot_symbol = 
		( (ORILIB_pilot_polarity_sequence[pilot_polarity_seq_counter] == 1) ? scaleFactor : -scaleFactor);

	tempIfftInput[43].real = pilot_symbol;
	tempIfftInput[43].imag = 0;
	tempIfftInput[57].real = pilot_symbol;
	tempIfftInput[57].imag = 0;
	tempIfftInput[7].real = pilot_symbol;
	tempIfftInput[7].imag = 0;
	tempIfftInput[21].real = -pilot_symbol;
	tempIfftInput[21].imag = 0;

	outStateBuf->pilot_polarity_counter = (pilot_polarity_seq_counter + 1)%127;

	/* Compute the IFFT */
	DSP_ifft16x16(
		ORILIB_twiddle_factors_ifft16x16_64,
		64,
		(int16_t *) tempIfftInput,
		(int16_t *) (ofdm_symbol + CYCLIC_PREFIX_LENGTH ) );

	/* extend cyclically -  */
	// Optimized version of this: memcpy(ofdm_symbol, ofdm_symbol + 64, CYCLIC_PREFIX_LENGTH * sizeof(Cplx16));
#pragma MUST_ITERATE(8,8,8);
	for (in_counter = 0; 
			in_counter < 16; 
			in_counter+=2)
	{
		_amem8(ofdm_symbol + in_counter) = _amem8(ofdm_symbol + 64 + in_counter);
	}

	/* multiply by window function */
	/* W(k): (81 samples)
	 * 0.5 for k = 0 and k = 80
	 * 1.0 for 1 <= k <= 79
	 */
	// [80] is first real (after cyclic prefix) [16] ...
	// [0] >> 1
	// [80] >> 1
		
}

void ORILIB_OFDMModulator_i_conf(
	CF ORILIB_OFDMModulator_t_Conf * conf,
	Uint32 scaleFactor
	){
	conf->scaleFactor = scaleFactor;
}
