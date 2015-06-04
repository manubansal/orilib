/**
Atomix project, ORILIB_EthWriter_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>

#include "ORILIB_EthWriter_Init_t.h"


void ORILIB_EthWriter_Init_i(
	OUT ORILIB_EthWriter_t_State * outStateBuf
	) {

  outStateBuf->vita_seqnum = 0;

}
