/**
Atomix project, ORILIB_SoftBranchMetricsR1by2_algorithms.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_SOFTBRANCHMETRICSR1BY2_ALGORITHMS_H_
#define ORILIB_SOFTBRANCHMETRICSR1BY2_ALGORITHMS_H_

#include <osl/inc/or_types.h>
#include <osl/inc/swpform.h>

void ORILIB_SoftBranchMetricsR1by2_calculate(
	IN  BitsSoft const * const depunctured_bits,
	OUT BitsSoft * const branch_metrics,
	IN  uint32_t FrameLength 
	);

#endif

