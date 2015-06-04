/**
Atomix project, ORILIB_SoftDeinterleaver48_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_SOFTDEINTERLEAVER48_I_H_

#include "ORILIB_SoftDeinterleaver48_t.h"

extern far const uint16_t WIFILIB_DeinterleaverLutWifi_BpskLUT[48];

void ORILIB_SoftDeinterleaver48_i (
	IN  ORILIB_t_SoftBitBuf48	* interleavedSoftBitsBuf,
	OUT ORILIB_t_SoftBitBuf48	* deinterleavedSoftBitsBuf
	);

#endif

