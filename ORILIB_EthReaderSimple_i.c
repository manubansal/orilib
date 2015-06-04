/**
Atomix project, ORILIB_EthReaderSimple_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include <osl/inc/amem4cpy_inl.h>
#include <osl/inc/eth/eth.h>
#include <osl/inc/uhd/uhd.h>
#include <osl/inc/eth/view_ale_table.h>
#include <string.h>
#include "ORILIB_EthReaderSimple_t.h"
#include "ORILIB_t.h"

#include <oros/sysilib/SYS_TimeStamp.h>

//#define ETH_HDR_LEN 14
//#define IP_HDR_LEN 20
//#define UDP_HDR_LEN 8
//#define TOTAL_HDR_LEN (ETH_HDR_LEN+IP_HDR_LEN+UDP_HDR_LEN)
//#define ETH_CRC_LEN 4
//
//#define ETH_SRC_IDX 6
//#define UDP_SRC_IDX 34

static uint8_t prev_seq = 0;
static Uint32 missing = 0;

void ORILIB_EthReaderSimple_i (
      OUT void * pkt,
      CF  ORILIB_EthReaderSimple_t_Conf * conf
      ) {
	Uint8* rx_packet = NULL;
	Uint32 rx_packet_len = 0;
	NET_ip_packet * ip_pkt = NULL;
	NET_udphdr * udphdr = NULL;

	//Uint8 mac[6];
	//eth_getMACAddress(mac);

		//!(rx_packet[26] == 0xAD && rx_packet[27] == 0x50))
	// wait until packet with UDP dst port 0xAD50
	while (rx_packet_len != conf->nDblWords*8 + 4 || udphdr->dest != conf->udpDstPort) {
	  eth_recv(&rx_packet, &rx_packet_len, 1);
	  ip_pkt = (NET_ip_packet *)(rx_packet);
	  udphdr = (NET_udphdr *)(ip_pkt->data);		//assuming no variable-length options in IP hdr
	  //eth_printMAC(mac);
	  DEBUG_ERROR(
	  printf("NODE_ID: %3d EthReaderSimple: rxPktLen: %d, ", NODE_ID, rx_packet_len);
	  printf(" dst:");
	  eth_printMAC(ip_pkt->hw_header.dst_eth);
	  printf(" src:");
	  eth_printMAC(ip_pkt->hw_header.src_eth);
	  printf(" udpDstPort: 0x%04x ", udphdr->dest);
	  eth_printUDPPayloadChars(ip_pkt, 4);
	  printf("\n");
	  )

	}
		
	DEBUG_ERROR(
	printf("* NODE_ID: %3d EthReaderSimple: rxPktLen: %d, ", NODE_ID, rx_packet_len);
	printf(" dst:");
	eth_printMAC(ip_pkt->hw_header.dst_eth);
	printf(" src:");
	eth_printMAC(ip_pkt->hw_header.src_eth);
	printf(" udpDstPort: 0x%04x ", udphdr->dest);
	eth_printUDPPayloadChars(ip_pkt, 4);
	printf("\n");
	)
	//_mem4cpy(sampleBuf, rx_packet + TOTAL_HDR_LEN, conf->nSamplesPerSymbol);
	_mem4cpy(pkt, rx_packet, conf->nDblWords * 2);

}

void ORILIB_EthReaderSimple_i_conf(
	CF  ORILIB_EthReaderSimple_t_Conf * conf,
	Uint32 nDblWords,
	Uint16 udpDstPort
	){
  conf->nDblWords = nDblWords;
  conf->udpDstPort = udpDstPort;
}
