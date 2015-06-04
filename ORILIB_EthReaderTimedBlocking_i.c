/**
Atomix project, ORILIB_EthReaderTimedBlocking_i.c, TODO: insert summary here
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
#include "ORILIB_EthReaderTimedBlocking_t.h"
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

void ORILIB_EthReaderTimedBlocking_i (
      OUT void * pkt,
      CF  ORILIB_EthReaderTimedBlocking_t_Conf * conf
      ) {
	Uint8* rx_packet = NULL;
	Uint32 rx_packet_len = 0;
	NET_ip_packet * ip_pkt = NULL;
	NET_udphdr * udphdr = NULL;

	volatile Uint64 entr_tsc = CSL_tscRead();
	volatile Uint64 exit_tsc = entr_tsc + conf->timeout;


	// wait until packet with UDP dst port 0xAD50 or timeout
	while (
		//(rx_packet_len != conf->nDblWords*8 + 4 || 
		(
		udphdr->dest != conf->udpDstPort
		) && CSL_tscRead() < exit_tsc
		){

	  eth_recv(&rx_packet, &rx_packet_len, 0);
	  ip_pkt = (NET_ip_packet *)(rx_packet);
	  udphdr = (NET_udphdr *)(ip_pkt->data);		//assuming no variable-length options in IP hdr
	  //eth_printMAC(mac);
	  DEBUG_ERROR(
	  printf("NODE_ID: %3d EthReaderTimedBlocking: rxPktLen: %d, ", NODE_ID, rx_packet_len);
	  printf(" dst:");
	  eth_printMAC(ip_pkt->hw_header.dst_eth);
	  printf(" src:");
	  eth_printMAC(ip_pkt->hw_header.src_eth);
	  printf(" udpDstPort: 0x%04x ", udphdr->dest);
	  eth_printUDPPayloadChars(ip_pkt, 4);
	  printf("\n");
	  )

	}

	Uint32 timeout = 1;

	while (CSL_tscRead() < exit_tsc) {
		timeout = 0;
	}

		
	DEBUG_ERROR(
	printf("eth reader: timeout = %d, %llu, %llu, %llu\n", timeout, entr_tsc, exit_tsc, CSL_tscRead());
	printf("* NODE_ID: %3d EthReaderTimedBlocking: rxPktLen: %d, ", NODE_ID, rx_packet_len);
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

void ORILIB_EthReaderTimedBlocking_i_conf(
	CF  ORILIB_EthReaderTimedBlocking_t_Conf * conf,
	Uint32 nDblWords,
	Uint16 udpDstPort,
	Uint64 timeout
	){
  conf->nDblWords = nDblWords;
  conf->udpDstPort = udpDstPort;
  conf->timeout = timeout;
}
