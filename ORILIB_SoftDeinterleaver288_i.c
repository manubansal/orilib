/**
Atomix project, ORILIB_SoftDeinterleaver288_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_SoftDeinterleaver_algorithms.h"
#include "ORILIB_SoftDeinterleaver288_i.h"

void ORILIB_SoftDeinterleaver288_i (
	IN  ORILIB_t_SoftBitBuf288	* interleavedSoftBitsBuf,
	OUT ORILIB_t_SoftBitBuf288	* deinterleavedSoftBitsBuf
	) {

	ORILIB_SoftDeinterleaver_lut_implementation (
	    interleavedSoftBitsBuf->softBits,
	    288,
	    WIFILIB_DeinterleaverLutWifi_64qamLUT,
	    deinterleavedSoftBitsBuf->softBits
	    );

	//DEBUG(
	//LOG_PRINTF("soft bits aftr SoftDeinterleaver288\n");
	//printOctetsDecSigned(deinterleavedSoftBitsBuf->softBits, 288);
	//)
}
