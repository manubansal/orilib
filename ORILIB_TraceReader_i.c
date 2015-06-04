/**
Atomix project, ORILIB_TraceReader_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <osl/inc/amem4cpy_inl.h>
#include "ORILIB_TraceReader_t.h"
#include "ORILIB_t.h"

#include <oros/sysilib/SYS_TimeStamp.h>

//#define N_WRAPAROUNDS 10
//#define N_WRAPAROUNDS 40
//#define N_WRAPAROUNDS 20
//#define N_WRAPAROUNDS 10

void ORILIB_TraceReader_i (
      IN  ORILIB_TraceReader_t_State * const inpStateBuf,
      OUT void * sampleBuf,
      OUT ORILIB_TraceReader_t_State * outStateBuf,
      CF  ORILIB_TraceReader_t_Conf * conf
      ) {

  static Uint32 nWraps = 0;
  Uint32 nextReadOffset = inpStateBuf->nextReadOffset; 

  //nSamples is the number of Cplx16 samples, that is, the number of Int16 pairs in the trace file
  //But nextReadOffset is incremented 2 units for each sample.
  if (nextReadOffset + conf->nSamplesPerSymbol * 2 > (conf->nSamples * 2)) {
    //if (nWraps < N_WRAPAROUNDS) {
    if (nWraps < conf->nWrapArounds) {
      nextReadOffset = 0;
      nWraps++;
    }
    else {
      LOG_PRINTF("ERROR: end of sample trace\n");
      SYS_TimeStampPrintAllAliased();
      exit(1);
    }
  }

  _amem4cpy(sampleBuf, &(conf->sampleTrace[nextReadOffset]), conf->nSamplesPerSymbol);
  nextReadOffset += conf->nSamplesPerSymbol * 2;		//factor of 2 is because the input trace is assumed to be Int16[], not Cplx16[]
  outStateBuf->nextReadOffset = nextReadOffset;
}

void ORILIB_TraceReader_i_conf(
	CF  ORILIB_TraceReader_t_Conf * conf,
	Int16 * sampleTrace,
	Uint32 nSamplesPerSymbol,
	Uint32 nSamples,
	Uint32 nWrapArounds
	){
  conf->sampleTrace = sampleTrace;
  conf->nSamplesPerSymbol = nSamplesPerSymbol;
  conf->nSamples = nSamples;
  conf->nWrapArounds = nWrapArounds;
}
