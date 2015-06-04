/**
Atomix project, ORILIB_SoftBranchMetricsR1by2F216_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_SoftBranchMetricsR1by2F216_t.h"
#include "ORILIB_SoftBranchMetricsR1by2_algorithms.h"

void ORILIB_SoftBranchMetricsR1by2F216_i (
	IN  ORILIB_t_SoftBitBuf432 * depuncturedSoftBitBuf,
	OUT ORILIB_t_SoftBitBuf432 * branchMetricBuf
	){


	ORILIB_SoftBranchMetricsR1by2_calculate (
	  depuncturedSoftBitBuf->softBits,
	  branchMetricBuf->softBits,
	  216
	  );


	DEBUG_DATA(
	LOG_PRINTF("ORILIB_SoftBranchMetricsR1by2F216: branch metrics\n");
	printOctetsDecSigned((Int8 *)branchMetricBuf->softBits, 216*2);
	)
}
