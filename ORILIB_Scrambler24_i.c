/**
Atomix project, ORILIB_Scrambler24_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_t.h"
#include "WIFILIB_t.h"
#include "ORILIB_Descrambler_algorithms.h"

void ORILIB_Scrambler24_i (
	IN  ORILIB_t_BitBucket24         * inpBuf,
	IN  ORILIB_t_Descrambler_State   * inpStateBuf,
	OUT ORILIB_t_BitBucket24         * outBuf,
	OUT ORILIB_t_Descrambler_State   * outStateBuf
	){

	// Just use the descrambler because they are the same
	ORILIB_Descrambler_i_internal (
	      (uint64_t *)inpBuf->packedBits,
	      inpStateBuf,
	      (uint64_t *)outBuf->packedBits,
	      outStateBuf,
	      kWifiGeeDataRatesInfo[kWifiGee6mbps].num_data_bits_per_ofdm_symbol,
	      1);

}


