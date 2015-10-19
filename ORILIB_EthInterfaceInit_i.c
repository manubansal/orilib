/**
Atomix project, ORILIB_EthInterfaceInit_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include <osl/inc/eth/eth.h>
#include "ORILIB_util.h"
#include "ORILIB_EthInterfaceInit_t.h"
#include <ti/platform/platform.h>

void ORILIB_EthInterfaceInit_i(
		CF ORILIB_t_EthInterfaceConf * conf
	) {

	uint8_t host_mac[6] = {0x00, 0x01, 0x01, 0x01, 0x01, 0x01};
	uint8_t amc_mac[6]  = {0x00, 0x01, 0x01, 0x01, 0x01, 0x02};
	uint8_t wire_mac[6] = {0x00, 0x01, 0x01, 0x01, 0x01, 0x03};

	platform_info p_I;
	platform_get_info(&p_I);
	printf("registering host mac address %02x:%02x:%02x:%02x:%02x:%02x\n",
	      p_I.emac.efuse_mac_address[0],
	      p_I.emac.efuse_mac_address[1],
	      p_I.emac.efuse_mac_address[2],
	      p_I.emac.efuse_mac_address[3],
	      p_I.emac.efuse_mac_address[4],
	      p_I.emac.efuse_mac_address[5]);


	//host_mac[4] = conf->nodeId;
	memcpy(host_mac, p_I.emac.efuse_mac_address, 6);
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
