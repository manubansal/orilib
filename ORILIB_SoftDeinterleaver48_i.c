/**
Atomix project, ORILIB_SoftDeinterleaver48_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include "ORILIB_SoftDeinterleaver_algorithms.h"
#include "ORILIB_SoftDeinterleaver48_i.h"

void ORILIB_SoftDeinterleaver48_i (
	IN  ORILIB_t_SoftBitBuf48	* interleavedSoftBitsBuf,
	OUT ORILIB_t_SoftBitBuf48	* deinterleavedSoftBitsBuf
	) {

	ORILIB_SoftDeinterleaver_lut_implementation (
	    interleavedSoftBitsBuf->softBits,
	    48,
	    WIFILIB_DeinterleaverLutWifi_BpskLUT,
	    deinterleavedSoftBitsBuf->softBits
	    );

	DEBUG_DATA(
	LOG_PRINTF("soft bits aftr SoftDeinterleaver48\n");
	printOctetsDecSigned(deinterleavedSoftBitsBuf->softBits, 48);
	)
}
