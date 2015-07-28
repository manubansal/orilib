/**
Atomix project, ORILIB_EthReaderBuffered_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_ETHREADERBUFFERED_I_H
#define ORILIB_ETHREADERBUFFERED_I_H 

#include "ORILIB_EthReaderBuffered_t.h"

      //OUT void * pkt,
void ORILIB_EthReaderBuffered_i (
      IN  ORILIB_t_EthReaderBuffer * state,
      CF  ORILIB_EthReaderBuffered_t_Conf * conf
      );

	//Int32 (*filter)(void *, Uint32)
void ORILIB_EthReaderBuffered_i_conf(
	CF  ORILIB_EthReaderBuffered_t_Conf * conf,
	Uint64 timeout,
	void (*filter)(void *, Uint32, Int32 *, Uint32 *),
	Int32 block_on_queue
	);

#endif /* ORILIB_ETHREADERBUFFERED_I_H */
