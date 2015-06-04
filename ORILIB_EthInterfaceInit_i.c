/**
Atomix project, ORILIB_EthInterfaceInit_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/eth/eth.h>

#include "ORILIB_EthInterfaceInit_t.h"

void ORILIB_EthInterfaceInit_i(
		CF ORILIB_t_EthInterfaceConf * conf
	) {

	uint8_t host_mac[6] = {0x00, 0x01, 0x01, 0x01, 0x01, 0x01};
	uint8_t amc_mac[6]  = {0x00, 0x01, 0x01, 0x01, 0x01, 0x02};
	uint8_t wire_mac[6] = {0x00, 0x01, 0x01, 0x01, 0x01, 0x03};


	host_mac[4] = conf->nodeId;
	amc_mac[4] = conf->nodeId;
	wire_mac[4] = conf->nodeId;

	int i;
//ADITYA : Commenting out printfs
//	printf("host_mac: ");
//	for (i = 0; i < 6; i ++) {
//	  printf("%02X ", host_mac[i]); 
//	}
//	printf("\n");
// End of commenting
	if (eth_init(host_mac, amc_mac, wire_mac) < 0)
	{
		DEBUG_ERROR(
		  printf("could not initialize ethernet interface\n");
		)
		SW_BREAKPOINT;
	}
}

void ORILIB_EthInterfaceInit_i_conf(
		CF ORILIB_t_EthInterfaceConf * conf,
		Uint8 nodeId
	) {
	conf->nodeId = nodeId;
}
