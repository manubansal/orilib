/**
Atomix project, ORILIB_EthReader_Reinit_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <osl/inc/eth/eth.h>
#include "ORILIB_EthReader_Reinit_t.h"

Uint64 arr[100];

void ORILIB_EthReader_Reinit_i(
	) {
	//inout, wire with +
	int i;
	Uint8* rx_packet;
	Uint32 rx_packet_len;

	CSL_tscEnable();

  DEBUG_INFO(
  LOG_PRINTF("ORILIB_EthReader_Reinit: lastReadCycle = 0\n");
  )

	// TODO: remove for timing testing
	/*for (i=0; i < 100; i++)
	*{
	*	arr[i] = CSL_tscRead();
	*	eth_recv(&rx_packet, &rx_packet_len);
	*	arr[i] = CSL_tscRead() - arr[i];
	}*/
}
