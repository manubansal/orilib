/**
Atomix project, ORILIB_EthReaderTimedBlocking_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_ETHREADERTIMEDBLOCKING_I_H
#define ORILIB_ETHREADERTIMEDBLOCKING_I_H 

#include "ORILIB_EthReaderTimedBlocking_t.h"

void ORILIB_EthReaderTimedBlocking_i (
      OUT void * pkt,
      CF  ORILIB_EthReaderTimedBlocking_t_Conf * conf
      );

void ORILIB_EthReaderTimedBlocking_i_conf(
	CF  ORILIB_EthReaderTimedBlocking_t_Conf * conf,
	Uint32 nDblWords,
	Uint16 udpDstPort,
	Uint64 timeout
	);

#endif /* ORILIB_ETHREADERTIMEDBLOCKING_I_H */
