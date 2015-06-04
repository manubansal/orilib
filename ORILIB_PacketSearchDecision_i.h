/**
Atomix project, ORILIB_PacketSearchDecision_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_PACKETSEARCHDECISION_I_H
#define ORILIB_PACKETSEARCHDECISION_I_H 

#include "ORILIB_t.h"
#include "ORILIB_PacketSearchDecision_t.h"

void ORILIB_PacketSearchDecision_i (
	IN  ORILIB_t_DetectState *bInpData,
	OUT ORILIB_t_PktAgcState *bOutData,
	OUT Decision_t *bOutDecision,
	CF  ORILIB_t_PacketSearchDecision_Conf *bInpConf
	);

void ORILIB_PacketSearchDecision_i_conf(
	CF  ORILIB_t_PacketSearchDecision_Conf *bInpConf,
	Uint32 st0,
	Uint32 st1
	);

#endif /* ORILIB_PACKETSEARCHDECISION_I_H */
