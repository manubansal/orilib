/**
Atomix project, ORILIB_TraceReader80_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef __ORILIB_TRACEREADER80_I_H__
#define __ORILIB_TRACEREADER80_I_H__

#include <osl/inc/swpform.h>
#include "ORILIB_TraceReader80_t.h"

void ORILIB_TraceReader80_i (
	IN  ORILIB_TraceReader80_t_State * const inpStateBuf,
	OUT ORILIB_t_Cplx16Buf80 * const restrict sampleBuf,
	OUT ORILIB_TraceReader80_t_State * const outStateBuf,
	CF  ORILIB_TraceReader80_t_Conf * conf
	); 


void ORILIB_TraceReader80_i_conf(
	CF  ORILIB_TraceReader80_t_Conf * conf,
	Int16 * sampleTrace,
	Uint32 nSamples,
	Uint32 nSamplesPerSec,
	Uint32 nWrapArounds,
	Uint64 throttleIntervalInCycles,
	Uint32 reportTimestamps
	);
	//char * confFile

#endif //__ORILIB_TRACEREADER80_I_H__
