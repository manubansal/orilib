/**
Atomix project, ORILIB_PacketSearchDecision_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_t.h"
#include "ORILIB_PacketSearchDecision_t.h"

void ORILIB_PacketSearchDecision_i (
	IN  ORILIB_t_DetectState *bInpData,
	OUT ORILIB_t_PktAgcState *bOutData,
	OUT Decision_t *bOutDecision,
	CF  ORILIB_t_PacketSearchDecision_Conf *bInpConf
	) {

  Uint32 nextState;
  Uint32 iterCount;

  Uint32 peakFound;
  Uint32 agcGain;

  Uint32 *transitionMap;
  transitionMap = bInpConf->transitionMap;

  peakFound = bInpData->peakFound;
  //agcGain   = bInpData->agcGain;
  //HACK
  agcGain = 0;

  bOutData->agcGain   = agcGain;

  if (peakFound) {
    nextState = transitionMap[0];
    iterCount = 1;
  }
  else {
    nextState = transitionMap[1];
    iterCount = 1;
  }

  bOutDecision->nextState = nextState;
  bOutDecision->iterCount = iterCount;

  DEBUG_DATA(
  LOG_PRINTF("ORILIB_PacketSearchDecision: peakFound=%d, agcGain=%d, nextState=%d, iterCount=%d\n", 
  	peakFound, agcGain, nextState, iterCount);
  )
}

void ORILIB_PacketSearchDecision_i_conf(
	CF  ORILIB_t_PacketSearchDecision_Conf *bInpConf,
	Uint32 st0,
	Uint32 st1
	) {
  Uint32 *transitionMap;
  transitionMap = bInpConf->transitionMap;

  transitionMap[0] = st0;
  transitionMap[1] = st1;
}
