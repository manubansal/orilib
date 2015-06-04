/**
Atomix project, ORILIB_Soft2Hard_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_SOFTRATE_S2H_I_H_
#define ORILIB_SOFTRATE_S2H_I_H_

#include <osl/inc/swpform.h>
#include "ORILIB_t.h"
#include "ORILIB_Softrate_s2h_t.h"

void Softrate_s2h_i(
		IN ORILIB_t_SoftBitBuf8	* inpSampleBuf,
		OUT ORILIB_t_ByteBuf8 *outBits
		);


#endif