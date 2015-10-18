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
#include "ORILIB_util.h"

//#define ETH_HDR_LEN 14
//#define IP_HDR_LEN 20
//#define UDP_HDR_LEN 8
//#define TOTAL_HDR_LEN (ETH_HDR_LEN+IP_HDR_LEN+UDP_HDR_LEN)
//#define ETH_CRC_LEN 4
//
//#define ETH_SRC_IDX 6
//#define UDP_SRC_IDX 34

static Uint32 prev_seq = 0, curr_seq = 0;
static Uint32 missing = 0;

#pragma DATA_ALIGN(aligned4PktBuf, 4);
static char aligned4PktBuf[1536];


static Uint32 savePkt(
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
  Uint32 blocked = 1;

  memcpy(aligned4PktBuf, pkt, 1536);
  pkt = aligned4PktBuf;

  //Int32 (*filter)(void *, Uint32);
  void (*filter)(void *, Uint32, Int32 * qid, Uint32 *);
  filter = conf->filter;
  //qid = (*filter)(pkt, pkt_len);
  (*filter)(pkt, pkt_len, &qid, &blocked);

  //DEBUG_INFO(
  //printf("savePkt\n");
  //printf("qid: %d\n", qid);
  //)

  if (qid < 0 || qid >= ORILIB_ETHREADERBUFFERED_N_QUEUES) {
    DEBUG_DATA(
      printf("bad queue id, dropping pkt...\n");
    )
    return blocked;
  }

  idx = (state->lastWritten[qid] + 1) % ORILIB_ETHREADERBUFFERED_N_BUFS;
  buf = state->pktBuf[qid][idx];
  nwr = state->nWritten[qid];

//  if (qid == 0) {
//	  memcpy(&curr_seq, pkt + 42, 4);
//	  printf("%d\n", curr_seq);
//	  missing = (curr_seq != (prev_seq + 1));
//	  prev_seq = curr_seq;
//  }

  if (nwr > (ORILIB_ETHREADERBUFFERED_N_BUFS >> 1 )) {
    //DEBUG_ERROR(
    //printf("queue %d full, dropping pkt...\n", qid);
    //printf("WARNING: queue %d more than half full...\n", qid);
    ORILIB_gpio_output_control(0, 5, 1);
    //)
  }
  else {
    ORILIB_gpio_output_control(0, 5, 0);
  }


  if (nwr == ORILIB_ETHREADERBUFFERED_N_BUFS) {
    //DEBUG_ERROR(
    //printf("queue %d full, dropping pkt...\n", qid);
    ORILIB_gpio_output_control(0, 6, 2);
    //)
  }
  else {
    ndw = ceil(pkt_len, 8);
  //#define ASSERT_PTR_ALIGNED(ptr, factor) assert(!((Uint32)ptr % factor))
  //  ASSERT_PTR_ALIGNED(pkt, 4);
  //  ASSERT_PTR_ALIGNED(buf, 4);
  //#undef ASSERT_PTR_ALIGNED
    _mem4cpy(buf, pkt, ndw * 2);
    nwr = nwr < ORILIB_ETHREADERBUFFERED_N_BUFS ? nwr + 1 : ORILIB_ETHREADERBUFFERED_N_BUFS;

    state->lastWritten[qid] = idx;
    state->nWritten[qid] = nwr;
  }

  if (conf->block_on_queue < 0) {
    blocked = 0;
  }
  else {
    blocked = (state->nWritten[conf->block_on_queue] == 0);
  }
  return blocked;
}

      //OUT void * pkt,
void ORILIB_EthReaderBuffered_i (
      IN  ORILIB_t_EthReaderBuffer * state,
      CF  ORILIB_EthReaderBuffered_t_Conf * conf
      ) {

	Uint8* rx_packet = NULL;
	Uint32 rx_packet_len = 0;
	Uint32 blocked = 1;

	NET_ip_packet * ip_pkt = NULL;
	NET_udphdr * udphdr = NULL;

	Uint32 nPktsRx = 0;
	Int32 ret = -1;

	volatile Uint64 entr_tsc = CSL_tscRead();
	volatile Uint64 exit_tsc = entr_tsc + conf->timeout;
	volatile Uint64 curr_tsc;

	curr_tsc = CSL_tscRead();


	if (conf->block_on_queue < 0) {
	  blocked = 0;
	}
	else {
	  blocked = (state->nWritten[conf->block_on_queue] == 0);
	}


	// wait until timeout
	while (blocked || (curr_tsc < exit_tsc)) {

	  if (blocked) {
	    ORILIB_gpio_output_control(0, 7, 1);
	  }
	  else {
	    ORILIB_gpio_output_control(0, 7, 0);
	  }

	  ret = eth_recv(&rx_packet, &rx_packet_len, 0);
	  curr_tsc = CSL_tscRead();
	  if (ret < 0)
	    continue;

	  nPktsRx++;
	  if (nPktsRx > 100) {
		  //SW_BREAKPOINT;
	  }
	  blocked &= savePkt(rx_packet, rx_packet_len, state, conf);

	  ip_pkt = (NET_ip_packet *)(rx_packet);
	  udphdr = (NET_udphdr *)(ip_pkt->data);		//assuming no variable-length options in IP hdr

	  //eth_printMAC(mac);
	  //DEBUG_ERROR(
	  //printf("NODE_ID: %3d EthReaderBuffered: rxPktLen: %d, ", NODE_ID, rx_packet_len);
	  //printf(" dst:");
	  //eth_printMAC(ip_pkt->hw_header.dst_eth);
	  //printf(" src:");
	  //eth_printMAC(ip_pkt->hw_header.src_eth);
	  //printf(" udpDstPort: 0x%04x ", udphdr->dest);
	  //eth_printUDPPayloadChars(ip_pkt, 4);
	  //printf("\n");
	  //)


	}

	if (blocked) {
	  ORILIB_gpio_output_control(0, 7, 1);
	}
	else {
	  ORILIB_gpio_output_control(0, 7, 0);
	}

	Uint32 timeout = 1;

	while (CSL_tscRead() < exit_tsc) {
		timeout = 0;
	}

	//printf("ORILIB_EthReaderBuffered: nPktsRx = %d\n", nPktsRx);

		
	//DEBUG_ERROR(
	//printf("eth reader: timeout = %d, %llu, %llu, %llu\n", timeout, entr_tsc, exit_tsc, CSL_tscRead());
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

//	Int32 (*filter)(void *, Uint32)
//block_on_queue < 0: don't block on any queue
//block_on_queue >= 0: block until qid = block_on_queue has at least one pkt
void ORILIB_EthReaderBuffered_i_conf(
	CF  ORILIB_EthReaderBuffered_t_Conf * conf,
	Uint64 timeout,
	void (*filter)(void *, Uint32, Int32 *, Uint32 *),
	Int32 block_on_queue
	){
  conf->timeout = timeout;
  conf->filter = filter;
  conf->block_on_queue = block_on_queue;
}
