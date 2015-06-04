/**
Atomix project, ORILIB_DebugPrintBitBucket_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_DebugPrintBitBucket_t.h"

void ORILIB_DebugPrintBitBucket_i (
	    IN  void const * const restrict inpBitBucket,
	    CF  ORILIB_DebugPrintBitBucketConf * conf
	    ){


	//DEBUG2(
	//DEBUG(
	//LOG_PRINTF("DebugPrintBitBucket:\n");
	//)
	//DEBUG3(
	DEBUG(
	LOG_PRINTF("DebugPrintBitBucket:\n");
	Uint32 index;
	Uint32 *packedBits = (Uint32 *)inpBitBucket;
	for (index = 0; index < conf->n; index++) {
		printBitsMsb(packedBits[index]);
		LOG_PRINTF("%d\n", packedBits[index]);
	}
	)
}

void ORILIB_DebugPrintBitBucket_i_conf (
	CF ORILIB_DebugPrintBitBucketConf * conf,
	Uint32 n
	){

	conf->n = n;
}

