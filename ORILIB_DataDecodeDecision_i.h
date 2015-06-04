/**
Atomix project, ORILIB_DataDecodeDecision_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_DATADECODEDECISION_I_H_
#define ORILIB_DATADECODEDECISION_I_H_

void ORILIB_DataDecodeDecision_i (
	IN  Vitdec_ViterbiDecodingMap const * const restrict vitdecMap,
	IN  ORILIB_t_DataDecodeState *bInpDecodeState,
	OUT ORILIB_t_DataDecodeState *bOutDecodeState,
	OUT Decision_t *bOutDecision,
	CF  ORILIB_t_DataDecodeDecisionConf *bInpConf
	);

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
	);
 

#endif

