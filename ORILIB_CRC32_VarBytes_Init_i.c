/**
Atomix project, ORILIB_CRC32_VarBytes_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include "ORILIB_CRC32_VarBytes_Init_t.h"

void ORILIB_CRC32_VarBytes_Init_i (
	IN  ORILIB_t_PLCPParserState		* plcpState,
	OUT ORILIB_t_CRC32_State 		* outState,
	CF  ORILIB_t_CRC32_VarBytes_Conf	* conf
	){

  Uint32 nDataBytesPerOfdmSymbol = conf->nDataBytesPerOfdmSymbol;
  Uint32 nDataBytesFirstSymbol = nDataBytesPerOfdmSymbol - 2;	//two bytes is service field
  Uint32 nOfdmSymsInPkt = plcpState->nOfdmSymsInPkt;		//for PHY payload part (excludes preamble and PLCP)
  Uint32 nDataBytesTotal = plcpState->packet_length_12bits;	//includes CRC32 in the length

  Uint32 nDataBytesTail = nOfdmSymsInPkt == 1 ? 
  	nDataBytesTotal : ((nDataBytesTotal - nDataBytesFirstSymbol) % nDataBytesPerOfdmSymbol);

  outState->crcValue = 0;

  outState->nBytes = nDataBytesTail;		//include CRC32 to end up with constant remainder
  //outState->nBytes = nDataBytesTail - 4;	//exclude CRC32 itself
}

void ORILIB_CRC32_VarBytes_Init_i_conf (
	CF  ORILIB_t_CRC32_VarBytes_Conf	* conf,
	IN  Uint32 nDataBytesPerOfdmSymbol
	){

  conf->nDataBytesPerOfdmSymbol = nDataBytesPerOfdmSymbol;
}
