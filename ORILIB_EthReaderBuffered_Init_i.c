/**
Atomix project, ORILIB_EthReaderBuffered_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_EthReaderBuffered_Init_t.h"

void ORILIB_EthReaderBuffered_Init_i (
      OUT  ORILIB_t_EthReaderBuffer * state
      ) {
      
  //char pktBuf[ORILIB_ETHREADERBUFFERED_N_QUEUES][N_BUFS][MSS]; 
  //Uint32 lastWritten[ORILIB_ETHREADERBUFFERED_N_QUEUES];
  //Uint32 nWritten[ORILIB_ETHREADERBUFFERED_N_QUEUES];

      Uint32 i;
      for (i = 0; i < ORILIB_ETHREADERBUFFERED_N_QUEUES; i++) {
        state->lastWritten[i] = 0;
	state->nWritten[i] = 0;
      }
}
