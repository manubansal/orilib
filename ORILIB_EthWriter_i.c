/**
Atomix project, ORILIB_EthWriter_i.c, TODO: insert summary here
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

#include <oros/sysilib/SYS_TimeStamp.h>

#define VITA_HDR_LEN 4
#define VITA_TRL_LEN 4
#define ETH_HDR_LEN 14
#define IP_HDR_LEN 20
#define UDP_HDR_LEN 8
#define TOTAL_HDR_LEN (ETH_HDR_LEN+IP_HDR_LEN+UDP_HDR_LEN)
#define ETH_CRC_LEN 4

#define VITA_HDR_WRD 1
#define VITA_TRL_WRD 1

#pragma DATA_ALIGN(tx_packet, 16)
#pragma DATA_SECTION(tx_packet, ".pkts")

#define ETH_DEST_IDX 0
#define UDP_DEST_IDX 36
#define IP_LEN_IDX 16
#define UDP_LEN_IDX 38
#define VITA_LEN_IDX 44
#define VITA_SEQNUM_IDX 43

static Uint8 tx_packet[1500];

static Uint8 hdrs[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,            // Dest MAC (0)
	0x00, 0x01, 0x01, 0x01, 0x01, 0x01,            // Src MAC (6)
	0x08, 0x00,                                    // Ethertype IPv4 (12)
  0x45, 0x00, 0x00, 0x00,                        // IP version, services, (14) total length (16)
  0x00, 0x00, 0x00, 0x00,                        // IP ID, flags, fragment offset 
  0x05, 0x11, 0x00, 0x00,                        // IP ttl, protocol (UDP), hdr checksum
  0xC0, 0xA8, 0x0A, 0x01,                        // Source IP address
  0xC0, 0xA8, 0x0A, 0x02,                        // Destination IP address
  0x00, 0x00, 0x00, 0x00,                        // UDP source port, dest port
  0x00, 0x00, 0x00, 0x00,                        // UDP len, UDP checksum
	0x04, 0x00, 0x00, 0x00                         // VRT header
};

static Uint8 debug_hdrs[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,            // Dest MAC (0)
	0x00, 0x01, 0x01, 0x01, 0x01, 0x01,            // Src MAC (6)
	0x00, 0x00,                                    // Ethertype 0
};

static Uint8 trailer[] = {
	0x00, 0xc0, 0x00, 0x00                         // VRT trailer
};

void ORILIB_EthWriter_i (
      IN  void * sampleBuf,
      IN  ORILIB_EthReader_t_State * const inOutRStateBuf,
      IN  ORILIB_EthWriter_t_State * const inOutWStateBuf,
      CF  ORILIB_EthWriter_t_Conf * conf
      ) {

	Uint32 idx = 0;

	// VITA
	if (conf->vitaOrDebug)
	{
		Uint16 ip_len, udp_len, vita_len;

		// fill in the packet template and samples
		_mem4cpy(tx_packet + idx, hdrs, sizeof(hdrs) >> 2);
		idx += sizeof(hdrs);
		_mem4cpy(tx_packet + idx, sampleBuf, conf->nSamplesPerSymbol);
		idx += conf->nSamplesPerSymbol << 2;
		_mem4cpy(tx_packet + idx, trailer, sizeof(trailer) >> 2);
		idx += sizeof(trailer);

		// fill in dynamic header fields
		
		// vita seqnum
		*((Uint8*)(tx_packet + VITA_SEQNUM_IDX)) = inOutWStateBuf->vita_seqnum++ & 0x0F;

		// src mac
		memcpy(tx_packet + ETH_DEST_IDX , &inOutRStateBuf->srcMAC, sizeof(inOutRStateBuf->srcMAC));

		// src udp port
		*((Uint16*)(tx_packet + UDP_DEST_IDX)) = inOutRStateBuf->srcUDPPort;

		// ip length 
		ip_len = IP_HDR_LEN + UDP_HDR_LEN + (conf->nSamplesPerSymbol << 2)	+ VITA_HDR_LEN + VITA_TRL_LEN;
		memcpy(tx_packet + IP_LEN_IDX, &ip_len, sizeof(ip_len));

		// udp length
		udp_len = ip_len - IP_HDR_LEN;
		memcpy(tx_packet + UDP_LEN_IDX, &udp_len, sizeof(udp_len));

		// vita length (words)
		vita_len = VITA_HDR_WRD + VITA_TRL_WRD + conf->nSamplesPerSymbol;
		memcpy(tx_packet + VITA_LEN_IDX, &vita_len, sizeof(vita_len));
		
		// TODO IP checksum
		// TODO UDP checksum

		eth_send(tx_packet, idx);
	}
	// Debug
	else
	{
		idx = 0;

		// copy Ethernet header template
		memcpy(tx_packet + idx, debug_hdrs, sizeof(debug_hdrs));
		idx += sizeof(debug_hdrs);

		// fill in dst MAC
		memcpy(tx_packet + ETH_DEST_IDX, &inOutRStateBuf->srcMAC, sizeof(inOutRStateBuf->srcMAC));

		// copy samples
		memcpy(tx_packet + idx, sampleBuf, conf->nSamplesPerSymbol*4);
		idx += conf->nSamplesPerSymbol*4;

		eth_send(tx_packet, idx);
	}
}

void ORILIB_EthWriter_i_conf(
	CF  ORILIB_EthWriter_t_Conf * conf,
	Uint32 nSamplesPerSymbol, 
	Uint8 vitaOrDebug
	){
  conf->nSamplesPerSymbol = nSamplesPerSymbol;
  conf->vitaOrDebug = vitaOrDebug;
}
