/**
Atomix project, ORILIB_EthWriterStatic_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_ETHWRITERSTATIC_I_H 

#include "ORILIB_EthWriterStatic_t.h"

void ORILIB_EthWriterStatic_i (
	CF ORILIB_t_EthWriterStaticConf * conf
      );

void ORILIB_EthWriterStatic_i_conf (
	CF ORILIB_t_EthWriterStaticConf * conf,
	IN char * pkt,
	IN Uint32 length_in_bytes
      );

#endif /* ORILIB_ETHWRITERSTATIC_I_H */
