/**
Atomix project, ORILIB_ByteCacheCappedInitFPlcpState_C432_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_ByteCacheInit_C432_t.h"
#include "WIFILIB_parameters.h"

void ORILIB_ByteCacheCappedInitFPlcpState_C432_i(
	IN  ORILIB_t_PLCPParserState * const restrict inpPlcpState,
	OUT ORILIB_t_ByteCacheCapped432	* outCache
	) {

	outCache->validLenInBytes = 0;
	outCache->totalConsumeCapInBytes = (inpPlcpState->packet_length_12bits * 8 //field is in bytes
	  + PHY_SERVICE_SIZE_BITS + PHY_TAIL_SIZE_BITS) * 2;
	outCache->totalConsumedInBytes = 0;
}
