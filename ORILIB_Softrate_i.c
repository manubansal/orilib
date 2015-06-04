/**
Atomix project, ORILIB_Softrate_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <src/DSP_ifft16x16/c66/DSP_ifft16x16.h>

#include "ORILIB_t.h"

#include "ORILIB_Softrate_i.h"

#include <math.h>

void ORILIB_Softrate_i(
			IN	ORILIB_t_SoftBitBuf48	* inpSampleBuf,
			IN	ORILIB_t_Softrate_State 	* inpStateBuf,
			OUT	ORILIB_t_Softrate_BER	* outSampleBuf,
			OUT	ORILIB_t_Softrate_State 	* outStateBuf
			) {
			
	BitsSoft const * const decoded_softbits = inpSampleBuf->softBits;
	ORILIB_t_Softrate_BER * computed_ber = outSampleBuf->softrate_ber;
	
	double metric_num = 0;
	uint32_t i;
	//uint32_t size = kWifiGeeDataRatesInfo[state->data_rate].num_coded_bits_per_ofdm_symbol;	
	uint32_t size = kWifiGeeDataRatesInfo[inpStateBuf->data_rate].num_coded_bits_per_ofdm_symbol;	
	for (i = 0; i < size; i++){
		metric_num += 1/(1+exp(_abs(decoded_softbits[i])));
	}
	
	outStateBuf->softrate_metric = inpStateBuf->softrate_metric + metric_num;
	outStateBuf->ofdm_count = inpStateBuf->ofdm_count + 1;
	outSampleBuf->softrate_ber = (outStateBuf->softrate_metric)/((outStateBuf->ofdm_count)*size);
	
}
