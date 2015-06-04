/**
Atomix project, ORILIB_FileTraceReader80_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include <osl/inc/swpform.h>
#include <osl/inc/amem4cpy_inl.h>
#include "ORILIB_FileTraceReader80_t.h"
#include "ORILIB_t.h"
#include <stdio.h>

#include <oros/sysilib/SYS_TimeStamp.h>

//#define N_WRAPAROUNDS 10
//#define N_WRAPAROUNDS 40
//#define N_WRAPAROUNDS 20
//#define N_WRAPAROUNDS 10

void ORILIB_FileTraceReader80_i (
      OUT ORILIB_t_Cplx16Buf80 * restrict sampleBuf,
      CF  ORILIB_FileTraceReader80_t_Conf * conf
      ) {

  //nSamples is the number of Cplx16 samples, that is, the number of Int16 pairs in the trace file
  //But nextReadOffset is incremented 2 units for each sample.
  Uint32 n;
  n = fread(sampleBuf, 4, 80, conf->f);
  if (n < 80) {
    LOG_PRINTF("ERROR: end of sample trace\n");
    fclose(conf->f);
    SYS_TimeStampPrintAllAliased();
    exit(1);
  }

  DEBUG_DATA(
  printCplx16(sampleBuf->samples, 80);
  )
}

void ORILIB_FileTraceReader80_i_conf(
	CF  ORILIB_FileTraceReader80_t_Conf * conf,
	char * traceFile
	){
  conf->f = fopen(traceFile, "rb");
  if (conf->f == NULL) {
    LOG_PRINTF("ERROR: could not fopen sample trace file: %s\n", traceFile);
    exit(1);
  }
}
