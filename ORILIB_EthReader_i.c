/**
Atomix project, ORILIB_EthReader_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/amem4cpy_inl.h>
#include <osl/inc/eth/eth.h>
#include <osl/inc/uhd/uhd.h>
#include <osl/inc/eth/view_ale_table.h>
#include <string.h>
#include "ORILIB_EthReader_t.h"
#include "ORILIB_t.h"

#include <oros/sysilib/SYS_TimeStamp.h>

#define ETH_HDR_LEN 14
#define IP_HDR_LEN 20
#define UDP_HDR_LEN 8
#define TOTAL_HDR_LEN (ETH_HDR_LEN+IP_HDR_LEN+UDP_HDR_LEN)
#define ETH_CRC_LEN 4

#define ETH_SRC_IDX 6
#define UDP_SRC_IDX 34

#pragma DATA_ALIGN(tx_packet, 16)
#pragma DATA_SECTION(tx_packet, ".pkts")
static Uint8 tx_packet[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                      /* Dest MAC */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                      /* Src MAC  */
  0x08, 0x00,                                              /* Ethertype = IPv4 */
  0x45, 0x00, 0x00, 0x00,                                  /* IP version, services, total length */
  0x00, 0x00, 0x00, 0x00,                                  /* IP ID, flags, fragment offset */
  0x05, 0x11, 0x32, 0x26,                                  /* IP ttl, protocol (UDP), hdr checksum */
  0x00, 0x00, 0x00, 0x01,                                  /* Source IP address */
  0x00, 0x00, 0x00, 0x00,                                  /* Destination IP address */
  0xAD, 0x50, 0xAD, 0x51,                                  /* UDP source port, dest port */
  0x00, 0x00, 0x00, 0x00,                                  /* UDP len, UDP checksum */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,          /* 24 bytes of payload data */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#pragma DATA_ALIGN(v49_data, 16)
#pragma DATA_SECTION(v49_data, ".pkts")
static vita49if_data v49_data;	

static uint8_t prev_seq = 0;
static Uint32 missing = 0;

void ORILIB_EthReader_i (
      IN  ORILIB_EthReader_t_State * const inpStateBuf,
      OUT void * sampleBuf,
      OUT ORILIB_EthReader_t_State * outStateBuf,
      CF  ORILIB_EthReader_t_Conf * conf
      ) {
	Uint8* rx_packet = NULL;
	Uint32 rx_packet_len = 0;

	// VITA
	if (conf->vitaOrDebug)
	{
		while (rx_packet == NULL ||
			vita49if_data_parse(&v49_data, rx_packet + TOTAL_HDR_LEN, rx_packet_len - TOTAL_HDR_LEN) < 0 ||
			v49_data.payload.len != conf->nSamplesPerSymbol*4)
		{
			eth_recv(&rx_packet, &rx_packet_len, 1);
		}
	
		if (v49_data.hdr.packet_count != ((prev_seq + 1) & 0xF))
			missing++;

		prev_seq = v49_data.hdr.packet_count;

		memcpy(outStateBuf->srcUDPPort, rx_packet + UDP_SRC_IDX, sizeof(outStateBuf->srcUDPPort));
		memcpy(outStateBuf->srcMAC, rx_packet + ETH_SRC_IDX, sizeof(outStateBuf->srcMAC));
		_mem4cpy(sampleBuf, v49_data.payload.buff, v49_data.payload.len >> 2);
	}
	// Debug
	else
	{
		// wait until packet with UDP dst port 0xAD50
		while (rx_packet_len < TOTAL_HDR_LEN + (conf->nSamplesPerSymbol*4) + ETH_CRC_LEN &&
			!(rx_packet[26] == 0xAD && rx_packet[27] == 0x50))
		{
			eth_recv(&rx_packet, &rx_packet_len, 1);
		}
		_mem4cpy(sampleBuf, rx_packet + TOTAL_HDR_LEN, conf->nSamplesPerSymbol);

		// send flow control packet
		memcpy(tx_packet, rx_packet+6, 6); // src is dst
		memcpy(tx_packet+6, rx_packet, 6); // dst is src
		eth_send(tx_packet, sizeof(tx_packet));
	}
}

void ORILIB_EthReader_i_conf(
	CF  ORILIB_EthReader_t_Conf * conf,
	Uint32 nSamplesPerSymbol, 
	Uint8 vitaOrDebug
	){
  conf->nSamplesPerSymbol = nSamplesPerSymbol;
  conf->vitaOrDebug = vitaOrDebug;
}
