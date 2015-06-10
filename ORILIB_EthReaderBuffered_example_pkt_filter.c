/**
Atomix project, ORILIB_EthReaderBuffered_example_pkt_filter.c, an example of a packet filter
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_EthReaderBuffered_t.h"

#include <osl/inc/swpform.h>
#include <osl/inc/eth/eth.h>
#include <string.h>
#include "ORILIB_t.h"


Int32 example_filter(void * pkt, Uint32 pkt_len) {
  Int32 qid = -1;
  //Int32 qid = 0;

  NET_ip_packet * ip_pkt = NULL;
  NET_udphdr * udphdr = NULL;

  ip_pkt = (NET_ip_packet *)(pkt);
  udphdr = (NET_udphdr *)(ip_pkt->data);		//assuming no variable-length options in IP hdr

  DEBUG_ERROR(
  printf("example_filter\n");
  //eth_printMAC(mac);
  printf("filter: NODE_ID: %3d EthReaderBuffered: rxPktLen: %d, ", NODE_ID, pkt_len);
  printf(" dst:");
  eth_printMAC(ip_pkt->hw_header.dst_eth);
  printf(" src:");
  eth_printMAC(ip_pkt->hw_header.src_eth);
  printf(" udpDstPort: 0x%04x ", udphdr->dest);
  eth_printUDPPayloadChars(ip_pkt, 4);
  printf("\n");
  )

  if (ip_pkt->protocol == 17) {	//udp (assuming it's an IP pkt)
    qid = 0;
  }
  else { 			//all others
    qid = 1;
  }

  return qid;
}
