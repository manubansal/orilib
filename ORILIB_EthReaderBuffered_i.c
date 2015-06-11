/**
Atomix project, ORILIB_EthReaderBuffered_i.c, TODO: insert summary here
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
#include "ORILIB_EthReaderBuffered_t.h"
#include "ORILIB_t.h"
#include <ti/csl/csl_tsc.h>

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


static void savePkt(
		void * pkt, 
		Uint32 pkt_len, 
		ORILIB_t_EthReaderBuffer * state, 
		ORILIB_EthReaderBuffered_t_Conf * conf
		) {
  char * buf;
  Uint32 idx;
  Uint32 nwr;
  Uint32 ndw;
  Int32 qid;

  Int32 (*filter)(void *, Uint32);
  filter = conf->filter;
  qid = (*filter)(pkt, pkt_len);

  //DEBUG_INFO(
  //printf("savePkt\n");
  //printf("qid: %d\n", qid);
  //)

  if (qid < 0 || qid >= N_QUEUES) {
    DEBUG_ERROR(
      printf("bad queue id, dropping pkt...\n");
    )
    return;
  }

  idx = (state->lastWritten[qid] + 1) % N_BUFS;
  buf = state->pktBuf[qid][idx];
  nwr = state->nWritten[qid];

  //DEBUG_ERROR(
  if (nwr == N_BUFS) {
    printf("queue %d full, overwriting pkt\n", qid);
  }
  //)

  ndw = ceil(pkt_len, 64);
  _mem4cpy(buf, pkt, ndw * 2);
  nwr = nwr < N_BUFS ? nwr + 1 : N_BUFS;

  state->lastWritten[qid] = idx;
  state->nWritten[qid] = nwr;
}

      //OUT void * pkt,
void ORILIB_EthReaderBuffered_i (
      IN  ORILIB_t_EthReaderBuffer * state,
      CF  ORILIB_EthReaderBuffered_t_Conf * conf
      ) {

	Uint8* rx_packet = NULL;
	Uint32 rx_packet_len = 0;

	NET_ip_packet * ip_pkt = NULL;
	NET_udphdr * udphdr = NULL;

	Uint32 nPktsRx = 0;
	Int32 ret = -1;

	volatile Uint64 entr_tsc = CSL_tscRead();
	volatile Uint64 exit_tsc = entr_tsc + conf->timeout;


	// wait until timeout
	while (CSL_tscRead() < exit_tsc) {

	  ret = eth_recv(&rx_packet, &rx_packet_len, 0);
	  if (ret < 0)
	    continue;

	  nPktsRx++;
	  savePkt(rx_packet, rx_packet_len, state, conf);

	  ip_pkt = (NET_ip_packet *)(rx_packet);
	  udphdr = (NET_udphdr *)(ip_pkt->data);		//assuming no variable-length options in IP hdr

	  //eth_printMAC(mac);
	  //DEBUG_ERROR(
	  printf("NODE_ID: %3d EthReaderBuffered: rxPktLen: %d, ", NODE_ID, rx_packet_len);
	  printf(" dst:");
	  eth_printMAC(ip_pkt->hw_header.dst_eth);
	  printf(" src:");
	  eth_printMAC(ip_pkt->hw_header.src_eth);
	  printf(" udpDstPort: 0x%04x ", udphdr->dest);
	  eth_printUDPPayloadChars(ip_pkt, 4);
	  printf("\n");
	  //)

	}

	Uint32 timeout = 1;

	while (CSL_tscRead() < exit_tsc) {
		timeout = 0;
	}

	printf("ORILIB_EthReaderBuffered: nPktsRx = %d\n", nPktsRx);

		
	//DEBUG_ERROR(
	printf("eth reader: timeout = %d, %llu, %llu, %llu\n", timeout, entr_tsc, exit_tsc, CSL_tscRead());
	//printf("* NODE_ID: %3d EthReaderBuffered: rxPktLen: %d, ", NODE_ID, rx_packet_len);
	//printf(" dst:");
	//eth_printMAC(ip_pkt->hw_header.dst_eth);
	//printf(" src:");
	//eth_printMAC(ip_pkt->hw_header.src_eth);
	//printf(" udpDstPort: 0x%04x ", udphdr->dest);
	//eth_printUDPPayloadChars(ip_pkt, 4);
	//printf("\n");
	//)


}

void ORILIB_EthReaderBuffered_i_conf(
	CF  ORILIB_EthReaderBuffered_t_Conf * conf,
	Uint64 timeout,
	Int32 (*filter)(void *, Uint32)
	){
  conf->timeout = timeout;
  conf->filter = filter;
}