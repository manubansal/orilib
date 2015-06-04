/**
Atomix project, ORILIB_PayloadParser_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_PayloadParser_t.h"

#define ORILIB_PayloadParser_seqs_size 2000
far Uint16 ORILIB_PayloadParser_seqs[ORILIB_PayloadParser_seqs_size];
far Uint64 ORILIB_PayloadParser_d8[ORILIB_PayloadParser_seqs_size];
far Uint32 ORILIB_PayloadParser_pktIdx = 0;

void ORILIB_PayloadParser_i (
	IN ORILIB_t_ByteCache2048	* inpCache
	){

	Int8 * pkt = inpCache->bytes;
	Uint8 hdr[16];
	Uint16 seq;
	Uint8 frag;


	util_swapBitsInBytes(pkt, hdr, 16);

	//LOG_PRINTF("PKT BYTES:\n");
	//printOctets(hdr, 16);

	seq = hdr[11];
	seq <<= 8;
	seq |= hdr[10];
	frag = seq & 0xf;
	seq >>= 4;
	//LOG_PRINTF("SEQNO: %04X\n", seq);

	ORILIB_PayloadParser_seqs[ORILIB_PayloadParser_pktIdx] = seq;
	memcpy(&ORILIB_PayloadParser_d8[ORILIB_PayloadParser_pktIdx], &hdr[8], 8);
	DEBUG_INFO(
	LOG_PRINTF("D8: %016llX\n", ORILIB_PayloadParser_d8[ORILIB_PayloadParser_pktIdx]);
	)
	ORILIB_PayloadParser_pktIdx++;
	ORILIB_PayloadParser_pktIdx = ORILIB_PayloadParser_pktIdx % ORILIB_PayloadParser_seqs_size;
}
