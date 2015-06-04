/**
Atomix project, ORILIB_Soft2Hard_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "softrate_s2h_i.h"
#include <src/DSP_ifft16x16/c66/DSP_ifft16x16.h>
#include <math.h>

void Softrate_s2h_i(
		IN ORILIB_t_SoftBitBuf8	* inpSampleBuf,
		OUT ORILIB_t_ByteBuf8 *outBits
		){

	BitsSoft const * const decoded_softbits = inpSampleBuf->softBits;
	uint32_t i;
	uint32_t size = 8;
	for (i = 0; i < size; i++){
		if (decoded_softbits[i] >= 0){
			outBits->bytes[i] = 1;
		}
		else{outBits->bytes[i] = 0;}
	}

}
