/**
Atomix project, ORILIB_TraceReaderN_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_TRACEREADERN_I_H 

#include "ORILIB_TraceReaderN_t.h"
#include "ORILIB_t.h"

void ORILIB_TraceReaderN_i (
      IN  ORILIB_TraceReaderN_t_State * const inpStateBuf,
      OUT void * restrict sampleBuf,
      OUT ORILIB_TraceReaderN_t_State * outStateBuf,
      CF  ORILIB_TraceReaderN_t_Conf * conf
      );

void ORILIB_TraceReaderN_i_conf(
	CF  ORILIB_TraceReaderN_t_Conf * conf,
	Int16 * sampleTrace,
	Uint32 nSamples,
	Uint32 nSamplesPerSec,
	Uint32 nWrapArounds,
	Uint64 throttleIntervalInCycles,
	Uint32 reportTimestamps,
	Uint32 nSamplesToReadPerCall
	);

#endif /* ORILIB_TRACEREADERN_I_H */
