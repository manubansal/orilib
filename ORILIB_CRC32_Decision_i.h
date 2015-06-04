/**
Atomix project, ORILIB_CRC32_Decision_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_CRC32_DECISION_I_H 

#include "ORILIB_CRC32_t.h"

void ORILIB_CRC32_Decision_i (
	IN  ORILIB_t_CRC32_State 	* inpState,
	OUT Decision_t 			* bOutDecision,
	CF  ORILIB_t_CRC32_DecisionConf * bInpConf
	);


void ORILIB_CRC32_Decision_i_conf (
	CF  ORILIB_t_CRC32_DecisionConf * bInpConf,
	IN  Uint32 stTxAck,
	IN  Uint32 stRxFinishWithoutTxAck
	);

#endif /* ORILIB_CRC32_DECISION_I_H */
