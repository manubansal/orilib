/**
Atomix project, ORILIB_EthWriter_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define __ORILIB_ETHWRITER_T_H__

#include <osl/inc/swpform.h>
#include "ORILIB_t.h"

typedef struct {
	Uint8 vita_seqnum;
} ORILIB_EthWriter_t_State;

typedef struct {
  Uint32 nSamplesPerSymbol;
	Uint8 vitaOrDebug;
} ORILIB_EthWriter_t_Conf;

#endif //__ORILIB_ETHWRITER_T_H__
