/**
Atomix project, ORILIB_Wait_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <oros/sysilib/SYS_TimeStamp.h>
#include <ti/csl/csl_tsc.h>
#include <stdio.h>

#include "ORILIB_Wait_t.h"


void ORILIB_Wait_i(
      CF  ORILIB_t_WaitConf * conf
      ) {

  Uint64 nCycles = conf->nCycles;
  Uint64 waitUntil = CSL_tscRead() + nCycles;
  volatile Uint64 tsc;

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
}

void ORILIB_Wait_i_conf(
      CF  ORILIB_t_WaitConf * conf,
      Uint64 nCycles
      ) {
      conf->nCycles = nCycles;
      CSL_tscEnable();
}
