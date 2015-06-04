/**
Atomix project, ORILIB_DataDecodeDecision_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_DataDecodeDecision_t.h"

#define step_size 4

void ORILIB_DataDecodeDecision_i (
	IN  Vitdec_ViterbiDecodingMap const * const restrict vitdecMap,
	IN  ORILIB_t_DataDecodeState *bInpDecodeState,
	OUT ORILIB_t_DataDecodeState *bOutDecodeState,
	OUT Decision_t *bOutDecision,
	CF  ORILIB_t_DataDecodeDecisionConf *bInpConf
	) {


  Uint32 nextState;
  Uint32 iterCount;

  ORILIB_t_DataDecodeMode dataDecodeMode;
  ORILIB_t_DataDecodeMode oldDataDecodeMode;

  Uint32 *transitionMap;
  transitionMap = bInpConf->transitionMap;

  dataDecodeMode = bInpDecodeState->dataDecodeMode;
  oldDataDecodeMode = dataDecodeMode;

  switch (dataDecodeMode) {
  case DATA_DECODE_STATE_INIT:
	if (vitdecMap->n0 > 1) {
	  dataDecodeMode = DATA_DECODE_STATE_CACHING_SYMS;
	  nextState = transitionMap[0];
	  iterCount = vitdecMap->n0 - 1;
	  break;
	}
	//else  (fallthrough)
  case DATA_DECODE_STATE_CACHING_SYMS:
	if (vitdecMap->n1 == 1) {	//single headtail
	  dataDecodeMode = DATA_DECODE_STATE_HEADTAIL_DECODING;
	  nextState = transitionMap[1];
	  iterCount = 1;
	}
	else {				//head convergent
	  if (vitdecMap->n2 != 1) {
	    DEBUG_ERROR(
	    LOG_PRINTF("ERROR: One of single-headtail or head-convergent must be true\n");
	    )
	  }
	  if (vitdecMap->n3 == 2) {
	    dataDecodeMode = DATA_DECODE_STATE_HEAD_DECODING_ONESYMRX;
	    nextState = transitionMap[9];
	  }
	  else {
	    dataDecodeMode = DATA_DECODE_STATE_HEAD_DECODING;
	    nextState = transitionMap[2];
	  }
	  iterCount = 1;
	}
	break;
  case DATA_DECODE_STATE_HEAD_DECODING_ONESYMRX:
      if (vitdecMap->n3 != 2) {
	DEBUG_ERROR(
	LOG_PRINTF("ERROR: not handled 1\n");
	)
	SYS_TimeStampPrintAllAliased();
	exit(1);
      }
      dataDecodeMode = DATA_DECODE_STATE_TAILfHEAD_DECODING_ONESYMRX;
      nextState = transitionMap[10];
      iterCount = 1;
      break;
  case DATA_DECODE_STATE_HEAD_DECODING:
  	if (vitdecMap->n3 >= 6) {	//mid convergent
	  dataDecodeMode = DATA_DECODE_STATE_MID_PRE_DECODING;
	  nextState = transitionMap[3];
	  iterCount = 1;
	}
	else {
	  if (vitdecMap->n3 != 2) {
	    DEBUG_ERROR(
	    LOG_PRINTF("ERROR: not handled 1\n");
	    )
	    SYS_TimeStampPrintAllAliased();
	    exit(1);
	  }
	  //n3 == 2 ==> two mid-convergent decodes total, which occur in the
	  //head convgt state
	  dataDecodeMode = DATA_DECODE_STATE_TAILfHEAD_DECODING;
	  nextState = transitionMap[6];
	  iterCount = 1;
	}
	break;
  case DATA_DECODE_STATE_MID_PRE_DECODING:
  	if (vitdecMap->n3 >= 10) {	//mid convergent
	  dataDecodeMode = DATA_DECODE_STATE_MID_DECODING;
	  nextState = transitionMap[4];
	  //iterCount = (vitdecMap->n3 - 6)/4;
	  iterCount = (vitdecMap->nMidIterations);
	}
	else {
	  LOG_PRINTF("ERROR: not handled 2\n");
	  SYS_TimeStampPrintAllAliased();
	  exit(1);
	}
	break;
  case DATA_DECODE_STATE_MID_DECODING:
  	dataDecodeMode = DATA_DECODE_STATE_TAIL_DECODING;
	nextState = transitionMap[5];
	iterCount = 1;
	break;
  case DATA_DECODE_STATE_HEADTAIL_DECODING:
	  dataDecodeMode = DATA_DECODE_STATE_TAIL_FINISH;
	  nextState = transitionMap[7];
	  iterCount = 1;
	  break;
  case DATA_DECODE_STATE_TAIL_DECODING:
	  dataDecodeMode = DATA_DECODE_STATE_TAIL_FINISH;
	  nextState = transitionMap[7];
	  iterCount = 1;
	  break;
  case DATA_DECODE_STATE_TAILfHEAD_DECODING_ONESYMRX:
	  dataDecodeMode = DATA_DECODE_STATE_TAILfHEAD_FINISH_ONESYMRX;
	  nextState = transitionMap[11];
	  iterCount = 1;
	  break;
  case DATA_DECODE_STATE_TAILfHEAD_DECODING:
	  dataDecodeMode = DATA_DECODE_STATE_TAILfHEAD_FINISH;
	  nextState = transitionMap[8];
	  iterCount = 1;
	  break;
  default:
  	LOG_PRINTF("ERROR: bad state in data decode decision\n");
	exit(1);
  }

  bOutDecision->nextState = nextState;
  bOutDecision->iterCount = iterCount;

  bOutDecodeState->dataDecodeMode = dataDecodeMode;

  DEBUG_VERBOSE(
  LOG_PRINTF("ORILIB_DataDecodeDecision: currState: %d, nextState=%d, iterCount=%d\n", 
  	oldDataDecodeMode, dataDecodeMode, iterCount);
  )
}

void ORILIB_DataDecodeDecision_i_conf (
	CF  ORILIB_t_DataDecodeDecisionConf *bInpConf,
	Uint32 stCachingSyms,
	Uint32 stHeadTailDecode,
	Uint32 stHeadDecode,
	Uint32 stMidPreDecode,
	Uint32 stMidDecode,
	Uint32 stTailDecode,
	Uint32 stTailfHeadDecode,
	Uint32 stTailFinish,
	Uint32 stTailfHeadFinish,
	Uint32 stHeadDecode_onesymrx,
	Uint32 stTailfHeadDecode_onesymrx,
	Uint32 stTailfHeadFinish_onesymrx
	) {
  Uint32 *transitionMap;
  transitionMap = bInpConf->transitionMap;

  transitionMap[0] = stCachingSyms;
  transitionMap[1] = stHeadTailDecode;
  transitionMap[2] = stHeadDecode;
  transitionMap[3] = stMidPreDecode;
  transitionMap[4] = stMidDecode;
  transitionMap[5] = stTailDecode;
  transitionMap[6] = stTailfHeadDecode;
  transitionMap[7] = stTailFinish;
  transitionMap[8] = stTailfHeadFinish;
  transitionMap[9] = stHeadDecode_onesymrx;
  transitionMap[10] = stTailfHeadDecode_onesymrx;
  transitionMap[11] = stTailfHeadFinish_onesymrx;
}
