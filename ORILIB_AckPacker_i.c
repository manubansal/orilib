/**
Atomix project, ORILIB_AckPacker_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/amem8cpy_inl.h>

#include "ORILIB_t.h"
#include "WIFILIB_lookupTables.h"
#include "ORILIB_CRC32_t.h"
#include "ORILIB_CRC32_algos.h"

#include "ORILIB_AckPacker_t.h"
#include "ORILIB_TailZeroer24_t.h"

#define MAC_ADDRESS_LEN 6

/* ACK 
 * 16 bits : the service field
 * 16 bits : frame control
 * 16 bits : duration
 * 48 bits : mac
 *    "    :  "
 *    "    :  "
 * 32 bits : FCS
 *    "    :  "
 * 16 bits : tail + pad
 * *just to make the length divisible by 4*
 */

void ORILIB_AckPacker_i(
            IN   ORILIB_t_ByteBuf8           * inpMacBuf,
            OUT  ORILIB_t_BitBucket256       * outBitsBuf,
						OUT  ORILIB_TailZeroer24_t_State * outTailZState
            ){
	Uint8* mac = inpMacBuf->bytes;
	Uint8* ack = outBitsBuf->packedBits;
	Uint32 crc = 0;
	Uint32 torev = 0;
	int i = 0;

	// Clear out the ACK template
	memset(ack, 0, 24);

	// SERVICE (starts at 2 so the rest is 4 byte aligned for CRC.
	// must skip 2 bytes in bitcache!)
	// leave as 0, scrambler will fill it
	
	// Frame control
	ack[4] = 0xD4;    // type/subtype

	// Duration (leave as 0)

	// RA
	_amem8cpy(ack+8, mac, 1);

	// Reverse the bits for the air (before CRC because the CRC algo assumes bits are in air order)
	#pragma MUST_ITERATE(5)
	for (i = 0; i < 5; i++) {
		_amem4(((Uint32*)ack)+i) = reverseBitsInBytes32(_amem4_const(((Uint32*)ack)+i));
	}

	// CRC (everything but the SERVICE field)
	ORILIB_CRC32_update(ack+4, 10, WIFILIB_crc32LookupTable, &crc);
	_amem2(ack+14) = _amem2_const(((Uint16*)&crc));
	_amem2(ack+16) = _amem2_const(((Uint16*)&crc)+1);;

	outTailZState->len = 128; // bits including service field and ending with CRC
}
