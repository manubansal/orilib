/**
Atomix project, ORILIB_Softrate_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_SOFTRATE_I_H_
#define ORILIB_SOFTRATE_I_H_

#include <osl/inc/swpform.h>

#include "ORILIB_Softrate_t.h"

void ORILIB_Softrate_i(
			IN	ORILIB_t_SoftBitBuf48	* inpSampleBuf,
			IN	Softrate_MetricState 	* inpStateBuf,
			OUT	ORILIB_t_BitBucket64	* outSampleBuf,
			OUT	Softrate_MetricState 	* outStateBuf
			);


#endif