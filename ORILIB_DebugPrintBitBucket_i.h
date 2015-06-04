/**
Atomix project, ORILIB_DebugPrintBitBucket_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_DEBUGPRINTBITBUCKET_I_H_
#define ORILIB_DEBUGPRINTBITBUCKET_I_H_

#include "ORILIB_DebugPrintBitBucket_t.h"

void ORILIB_DebugPrintBitBucket_i (
	    IN  void const * const restrict inpBitBucket,
	    CF  ORILIB_DebugPrintBitBucketConf * conf
	    );


void ORILIB_DebugPrintBitBucket_i_conf (
	CF ORILIB_DebugPrintBitBucketConf * conf,
	Uint32 n
	);


#endif

