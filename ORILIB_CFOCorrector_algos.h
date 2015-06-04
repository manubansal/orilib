/**
Atomix project, ORILIB_CFOCorrector_algos.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_CFOCORRECTOR_ALGOS_H
#define ORILIB_CFOCORRECTOR_ALGOS_H 

#include "ORILIB_t.h"

void ORILIB_CFOCorrectorG_inner(
		IN	Cplx16			const * const restrict inSampleBuf,
		IN	ORILIB_t_CFOState 	const * const restrict cfoState,
		IN	ORILIB_t_PktAgcState 	const * const restrict agcState, 
		OUT	Cplx16			* const restrict outSampleBuf
		);

void ORILIB_CFOCorrectorGM_inner(
		IN	Cplx16			const * const restrict inSampleBuf,
		IN	ORILIB_t_CFOState 	const * const restrict cfoState,
		IN	ORILIB_t_PktAgcState 	const * const restrict agcState, 
		OUT	Cplx16			* const restrict outSampleBuf
		);

#endif /* ORILIB_CFOCORRECTOR_ALGOS_H */
