/**
Atomix project, ORILIB_PacketSearchDecision_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_PACKETSEARCHDECISION_T_H 

#include "ORILIB_t.h"
#include <oros/sysilib/Decision_t.h>		//for Decision_t type

#define ORILIB_PACKETSEARCHDECISION_N_DECWAYS 2

typedef struct {
  Uint32 transitionMap[ORILIB_PACKETSEARCHDECISION_N_DECWAYS];
} ORILIB_t_PacketSearchDecision_Conf;

#endif /* ORILIB_PACKETSEARCHDECISION_T_H */
