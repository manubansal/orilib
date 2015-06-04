/**
Atomix project, ORILIB_EthReader_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#ifndef __ORILIB_ETHREADER_T_H__
#define __ORILIB_ETHREADER_T_H__

#include <osl/inc/swpform.h>
#include "ORILIB_t.h"
#include <osl/inc/eth/eth.h>

typedef struct {
	Uint8 srcMAC[6];
	Uint16 srcUDPPort;
} ORILIB_EthReader_t_State;

typedef struct {
  Uint32 nSamplesPerSymbol;
  Uint8 vitaOrDebug;
} ORILIB_EthReader_t_Conf;

typedef struct {
  Uint32 nDblWords;
  Uint16 udpDstPort;
} ORILIB_EthReaderSimple_t_Conf;

typedef struct {
  Uint32 nDblWords;
  Uint16 udpDstPort;
  Uint64 timeout;
} ORILIB_EthReaderTimedBlocking_t_Conf;

#endif //__ORILIB_ETHREADER_T_H__
