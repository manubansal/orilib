/**
Atomix project, ORILIB_CRC32_VarBytes_Init_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_CRC32_VARBYTES_INIT_I_H 

#include "ORILIB_CRC32_VarBytes_Init_t.h"

void ORILIB_CRC32_VarBytes_Init_i (
	IN  ORILIB_t_PLCPParserState		* plcpState,
	OUT ORILIB_t_CRC32_State 		* outState,
	CF  ORILIB_t_CRC32_VarBytes_Conf	* conf
	);


void ORILIB_CRC32_VarBytes_Init_i_conf (
	CF  ORILIB_t_CRC32_VarBytes_Conf	* conf,
	IN  Uint32 nDataBytesPerOfdmSymbol
	);

#endif /* ORILIB_CRC32_VARBYTES_INIT_I_H */
