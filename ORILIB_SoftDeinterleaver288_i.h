/**
Atomix project, ORILIB_SoftDeinterleaver288_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_SOFTDEINTERLEAVER288_I_H_
#define ORILIB_SOFTDEINTERLEAVER288_I_H_

#include "ORILIB_SoftDeinterleaver288_t.h"

extern far const uint16_t WIFILIB_DeinterleaverLutWifi_64qamLUT[288];

void ORILIB_SoftDeinterleaver288_i (
	IN  ORILIB_t_SoftBitBuf288	* interleavedSoftBitsBuf,
	OUT ORILIB_t_SoftBitBuf288	* deinterleavedSoftBitsBuf
	);

#endif

