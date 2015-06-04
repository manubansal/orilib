/**
Atomix project, ORILIB_PLCPParser_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_PLCPPARSER_I_H_

#include "ORILIB_PLCPParser_t.h"

void ORILIB_PLCPParser_i (
	    IN  ORILIB_t_ByteBuf4 const * const restrict inpBufPLCP,
	    OUT ORILIB_t_PLCPParserState * const restrict state,
	    OUT WIFILIB_TxRxPktState * const restrict outPktState,
	    OUT Decision_t *bOutDecision,
	    CF  ORILIB_t_PLCPParserConf *conf
	    );

void ORILIB_PLCPParser_i_conf (
	CF  ORILIB_t_PLCPParserConf *conf,
	Uint32 st6m,
	Uint32 st54m,
	Uint32 st54m_onesymrx
	);

#endif

