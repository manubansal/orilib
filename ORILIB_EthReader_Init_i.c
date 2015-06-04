/**
Atomix project, ORILIB_EthReader_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include <osl/inc/eth/eth.h>

#include "ORILIB_EthReader_Init_t.h"


void ORILIB_EthReader_Init_i(
	OUT ORILIB_EthReader_t_State * outStateBuf
	) {

	//uint8_t host_mac[6] = {0x00, 0x01, 0x01, 0x01, 0x01, 0x01};
	//uint8_t amc_mac[6]  = {0x00, 0x01, 0x01, 0x01, 0x01, 0x02};
	//uint8_t wire_mac[6] = {0x00, 0x01, 0x01, 0x01, 0x01, 0x03};

	//if (eth_init(host_mac, amc_mac, wire_mac) < 0)
	//{
	//	SW_BREAKPOINT;
	//}

	//I moved eth init function to the ORILIB_EthInterfaceInit block. --MB

	outStateBuf->srcUDPPort = 5552;
}
