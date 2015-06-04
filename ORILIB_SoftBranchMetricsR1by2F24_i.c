/**
Atomix project, ORILIB_SoftBranchMetricsR1by2F24_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/



#include "ORILIB_SoftBranchMetricsR1by2F24_t.h"
#include "ORILIB_SoftBranchMetricsR1by2_algorithms.h"

void ORILIB_SoftBranchMetricsR1by2F24_i (
	IN  ORILIB_t_SoftBitBuf48 * depuncturedSoftBitBuf,
	OUT ORILIB_t_SoftBitBuf48 * branchMetricBuf
	){


	ORILIB_SoftBranchMetricsR1by2_calculate (
	  depuncturedSoftBitBuf->softBits,
	  branchMetricBuf->softBits,
	  24
	  );


	DEBUG_DATA(
	LOG_PRINTF("ORILIB_SoftBranchMetricsR1by2F24: branch metrics\n");
	printOctetsDecSigned((Int8 *)branchMetricBuf->softBits, 48);
	)
}
