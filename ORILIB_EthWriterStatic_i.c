/**
Atomix project, ORILIB_EthWriterStatic_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include <osl/inc/amem4cpy_inl.h>
#include <osl/inc/eth/eth.h>
#include <string.h>
#include "ORILIB_EthReader_t.h"
#include "ORILIB_EthWriter_t.h"
#include "ORILIB_t.h"

#include "ORILIB_EthWriterStatic_t.h"

void ORILIB_EthWriterStatic_i (
	CF ORILIB_t_EthWriterStaticConf * conf
      ) {



	NET_ip_packet * ip_pkt = NULL;
	NET_udphdr * udphdr = NULL;
	//Uint8 mac[6];

	eth_send(conf->pkt, conf->length_in_bytes);

	ip_pkt = (NET_ip_packet *)(conf->pkt);
	udphdr = (NET_udphdr *)(ip_pkt->data);		//assuming no variable-length options in IP hdr
	//eth_getMACAddress(mac);
	//eth_printMAC(mac);
	DEBUG_ERROR(
	printf("NODE_ID: %3d EthWriterStatic: txPktLen: %d, ", NODE_ID, conf->length_in_bytes);
	printf(" dst:");
	eth_printMAC(ip_pkt->hw_header.dst_eth);
	printf(" src:");
	eth_printMAC(ip_pkt->hw_header.src_eth);
	printf(" udpDstPort: 0x%04x\n", udphdr->dest);
	)
}


void ORILIB_EthWriterStatic_i_conf (
	CF ORILIB_t_EthWriterStaticConf * conf,
	IN char * pkt,
	IN Uint32 length_in_bytes
      ) {
      conf->pkt = pkt;
      conf->length_in_bytes = length_in_bytes;
}
