/**
Atomix project, ORILIB_EthReader_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define __ORILIB_ETHREADER_I_H__

#include <osl/inc/swpform.h>
#include "ORILIB_EthReader_t.h"

void ORILIB_EthReader_i (
	IN  ORILIB_EthReader_t_State * const inpStateBuf,
	OUT void* sampleBuf,
	OUT ORILIB_EthReader_t_State * const outStateBuf,
	CF  ORILIB_EthReader_t_Conf * conf
	); 


void ORILIB_EthReader_i_conf(
	CF  ORILIB_EthReader_t_Conf * conf,
	Uint32 nSamplesPerSymbol,
	Uint8 vitaOrDebug
	);

#endif //__ORILIB_ETHREADER_I_H__
