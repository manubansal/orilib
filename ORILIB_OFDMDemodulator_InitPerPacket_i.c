/**
Atomix project, ORILIB_OFDMDemodulator_InitPerPacket_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_OFDMDemodulator_t.h"

void ORILIB_OFDMDemodulator_InitPerPacket_i(
	OUT ORILIB_OFDMDemodulator_t_State * outState
	) {

	outState->pilot_polarity_counter = 0;
}
