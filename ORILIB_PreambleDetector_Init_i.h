/**
Atomix project, ORILIB_PreambleDetector_Init_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_PREAMBLEDETECTOR_INIT_I_H
#define ORILIB_PREAMBLEDETECTOR_INIT_I_H 

#include "ORILIB_t.h"

void ORILIB_PreambleDetector_Init_i(
	OUT ORILIB_t_CorrState   * corrState,
	OUT ORILIB_t_PktAgcState * gainState 
	);

#endif /* ORILIB_PREAMBLEDETECTOR_INIT_I_H */
