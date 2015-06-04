/**
Atomix project, ORILIB_EthInterfaceInit_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_ETHINTERFACEINIT_I_H 

#include "ORILIB_EthInterfaceInit_t.h"

void ORILIB_EthInterfaceInit_i(
		CF ORILIB_t_EthInterfaceConf * conf
	);


void ORILIB_EthInterfaceInit_i_conf(
		CF ORILIB_t_EthInterfaceConf * conf,
		Uint8 nodeId
		);

#endif /* ORILIB_ETHINTERFACEINIT_I_H */
