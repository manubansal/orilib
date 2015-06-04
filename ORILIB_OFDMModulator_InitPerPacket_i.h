/**
Atomix project, ORILIB_OFDMModulator_InitPerPacket_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_OFDMMODULATOR_INITPERPACKET_I_H_

#include <osl/inc/swpform.h>
#include "ORILIB_OFDMModulator_t.h"
#include "ORILIB_OFDMModulator_InitPerPacket_t.h"

void ORILIB_OFDMModulator_InitPerPacket_i(
	OUT ORILIB_OFDMModulator_t_State * outState,
	CF  ORILIB_OFDMModulator_InitPerPacket_t_Conf * conf
	);

void ORILIB_OFDMModulator_InitPerPacket_i_conf(
	CF  ORILIB_OFDMModulator_InitPerPacket_t_Conf * conf,
	Uint32 pilot_polarity_counter
	);

#endif

