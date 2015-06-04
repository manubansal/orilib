/**
Atomix project, ORILIB_EthWriter_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define __ORILIB_ETHWRITER_I_H__

#include <osl/inc/swpform.h>
#include "ORILIB_EthWriter_t.h"
#include "ORILIB_EthReader_t.h"

void ORILIB_EthWriter_i (
      IN  void * sampleBuf,
      IN  ORILIB_EthReader_t_State * const inOutRStateBuf,
      IN	ORILIB_EthWriter_t_State * const inOutWStateBuf,
      CF  ORILIB_EthWriter_t_Conf * conf
      );


void ORILIB_EthWriter_i_conf(
	CF  ORILIB_EthWriter_t_Conf * conf,
	Uint32 nSamplesPerSymbol,
	Uint8 vitaOrDebug
	);

#endif //__ORILIB_ETHWRITER_I_H__
