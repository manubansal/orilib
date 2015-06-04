/**
Atomix project, ORILIB_TraceReader_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define __ORILIB_TRACEREADER_I_H__

#include <osl/inc/swpform.h>
#include "ORILIB_TraceReader_t.h"

void ORILIB_TraceReader_i (
	IN  ORILIB_TraceReader_t_State * const inpStateBuf,
	OUT void* sampleBuf,
	OUT ORILIB_TraceReader_t_State * const outStateBuf,
	CF  ORILIB_TraceReader_t_Conf * conf
	); 


void ORILIB_TraceReader_i_conf(
	CF  ORILIB_TraceReader_t_Conf * conf,
	Int16 * sampleTrace,
        Uint32 nSamplesPerSymbol,
	Uint32 nSamples,
	Uint32 nWrapArounds
	);

#endif //__ORILIB_TRACEREADER80_I_H__
