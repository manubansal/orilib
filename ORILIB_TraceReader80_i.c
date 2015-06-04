/**
Atomix project, ORILIB_TraceReader80_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <osl/inc/amem4cpy_inl.h>
#include "ORILIB_TraceReader80_t.h"
#include "ORILIB_t.h"

#include <oros/sysilib/SYS_TimeStamp.h>
#include <ti/csl/csl_tsc.h>
#include <stdio.h>

//#define NT 100

void ORILIB_TraceReader80_i (
      IN  ORILIB_TraceReader80_t_State * const inpStateBuf,
      OUT ORILIB_t_Cplx16Buf80 * restrict sampleBuf,
      OUT ORILIB_TraceReader80_t_State * outStateBuf,
      CF  ORILIB_TraceReader80_t_Conf * conf
      ) {

  //far static volatile Uint64 inCycle[NT];
  //far static volatile Uint64 outCycle[NT];
  //far static Uint32 count = 0;
  //far static Uint32 tsidx = 0;

  //far static Uint32 nWraps = 0;
  //far static Uint32 nSamplesRead = 0;
  Uint32 nWraps = inpStateBuf->nWraps;
  Uint32 nSamplesRead = inpStateBuf->nSamplesRead;
  Uint32 nextReadOffset = inpStateBuf->nextReadOffset; 
  Uint64 throttleIntervalInCycles = conf->throttleIntervalInCycles;
  Uint64 lastReadCycle = inpStateBuf->lastReadCycle;
  Uint64 waitUntil;
  volatile Uint64 tsc;
  if (lastReadCycle == 0) {
    waitUntil = 0;
  }
  else {
    waitUntil = lastReadCycle + throttleIntervalInCycles;
  }

  //throttle wait
#ifdef TS_FIFO_WAIT_I
	SYS_TimeStamp_aliased(1135ULL);
#endif
  do {
    tsc = CSL_tscRead();
  } while (tsc < waitUntil);
#ifdef TS_FIFO_WAIT_I
	SYS_TimeStamp_aliased(1136ULL);
#endif

  //inCycle[tsidx] = CSL_tscRead();
  if (conf->reportTimestamps)
    SYS_TimeStamp_aliased(1234);

  DEBUG_INFO(
  if (conf->nSamplesPerSec && (nSamplesRead % conf->nSamplesPerSec == 0)) {
    LOG_PRINTF("TraceReader80: ------------ Start of new section ------------\n");
    //SW_BREAKPOINT;
  }
  LOG_PRINTF("TraceReader80: nSamplesRead = %d \n", nSamplesRead);
  )

  //nSamples is the number of Cplx16 samples, that is, the number of Int16 pairs in the trace file
  //But nextReadOffset is incremented 2 units for each sample.
  if (nextReadOffset + 80 * 2 > (conf->nSamples * 2)) {
    //if (nWraps < N_WRAPAROUNDS) {
    if (conf->nWrapArounds == 0xFFFFFFFF || nWraps < conf->nWrapArounds) {
      nextReadOffset = 0;
      nWraps++;
    }
    else {
      LOG_PRINTF("ERROR: end of sample trace\n");
      SYS_TimeStampPrintAllAliased();
      exit(1);
    }
  }

  _amem4cpy(sampleBuf->samples, &(conf->sampleTrace[nextReadOffset]), 80);
  nextReadOffset += 80 * 2;		//factor of 2 is because the input trace is assumed to be Int16[], not Cplx16[]
  nSamplesRead += 80;
  outStateBuf->nextReadOffset = nextReadOffset;
  outStateBuf->nWraps = nWraps;
  outStateBuf->nSamplesRead = nSamplesRead;

  //outStateBuf->lastReadCycle = tsc; 
  //This is erroneous since it models a one-buffer deep dependency. However, we
  //are assuming a deep-queue model where nth buffer is available to read at
  //time t = tau * n + t0, where tau is the throttle interval and t0 is the
  //time of availability of the zero-th buffer. Initial value of lastReadCycle
  //is t0 in this case, and for rest of the model to work out, we just keep
  //incrementing it by tau (instead of the actual read cycle), as in the
  //following:

  if (lastReadCycle == 0) {
    lastReadCycle = tsc;
  }
  else {
    lastReadCycle += throttleIntervalInCycles;
  }
  outStateBuf->lastReadCycle = lastReadCycle; 

  DEBUG_ALL(
  printCplx16(sampleBuf->samples, 80);
  )

  //outCycle[tsidx] = CSL_tscRead();
  //count++;
  //tsidx = (tsidx + 1) % NT;


  if (conf->reportTimestamps)
    SYS_TimeStamp_aliased(1235);
}

void ORILIB_TraceReader80_i_conf(
	CF  ORILIB_TraceReader80_t_Conf * conf,
	Int16 * sampleTrace,
	Uint32 nSamples,
	Uint32 nSamplesPerSec,
	Uint32 nWrapArounds,
	Uint64 throttleIntervalInCycles,
	Uint32 reportTimestamps
	){
	//char * confFile
  conf->sampleTrace = sampleTrace;
  conf->nSamples = nSamples;
  conf->nSamplesPerSec = nSamplesPerSec;
  conf->nWrapArounds = nWrapArounds;
  conf->throttleIntervalInCycles = throttleIntervalInCycles;
  conf->reportTimestamps = reportTimestamps;

  //Uint32 f_ns;
  //LOG_PRINTF("TraceReader80: Trying to read conf file %s\n", confFile);
  //FILE *f = fopen(confFile, "rb");
  //if (f > 0) {
  //  printf("TraceReader80: Found conf file\n");
  //  fread(&f_ns, 4, 1, f);
  //  LOG_PRINTF("TraceReader80: f_ns=%u\n", f_ns);
  //  conf->nSamples = f_ns;
  //  fclose(f);
  //}
  //else {
  //  LOG_PRINTF("TraceReader80: No conf file found\n");
  //}
  DEBUG_INFO(
  LOG_PRINTF("TraceReader80: conf->nSamples = %u\n", conf->nSamples);
  )
}
