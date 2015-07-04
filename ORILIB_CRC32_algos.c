/**
Atomix project, ORILIB_CRC32_algos.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include <osl/inc/swpform.h>
#include <osl/inc/util.h>

#include "ORILIB_CRC32_algos.h"

#define lsb_OLDEST	1
#define msb_OLDEST	2

//#define CRC32_BITS_IN_BYTE_ORDER	lsb_OLDEST
#define CRC32_BITS_IN_BYTE_ORDER	msb_OLDEST

#define N_BITS_IN_CRC				32




/** Block-wise implementation of the crc algorithm. It returns and accepts the
 * current crc value, which is used as state to continue a previous crc computation.
 *
 * For the first call, initialize arg_CrcValue to zero.
 */
void ORILIB_CRC32_update (
		IN Uint8 InputBytes[restrict],
		IN Uint32 NumInputBytes,
		IN Uint32 CRCTable[restrict],
		INOUT	Uint32 *arg_CrcValue
){

	Uint8 Byte1, Byte2, Byte3, Byte4;
	Uint32 CrcValue;
	Uint32 InputWord,NumInputWords,WordCnt;
	Uint32 *pInputWords;
	Uint32 NumNonWordBytes;

	/* initialize CRC */
	//    CrcValue = 0;
	/* wifi-specific modification to account for part (a) of the FCS
	 * terms as in IEEE 802.11-2007 sec 7.1.3.7.
	 */
	//	CrcValue = 0xFFFFFFFF;

	CrcValue = *arg_CrcValue;

#if (CRC32_BITS_IN_BYTE_ORDER == lsb_OLDEST)
	CrcValue = reverseBitsInBytes32(CrcValue);
#endif
	CrcValue ^= 0xFFFFFFFF;

	/*  assign pointers */
	pInputWords = ( Uint32 * ) &InputBytes[0];

	/*   compute the number of input words
	 *   round up to an integer number of words
	 */
	NumInputWords = NumInputBytes >> 2;
	NumNonWordBytes = NumInputBytes & 3;;

#ifdef _TMS320C6X
	//    _nassert((int)pInputWords % 8 == 0);
	//    #pragma MUST_ITERATE(2, ,);
#endif


	for( WordCnt = 0; WordCnt < NumInputWords; WordCnt++ )
	{
		/* The following is assuming that the input hex bytes are written such that
		 * in each byte, lsb is the oldes (first to be transmitted). This is the
		 * convention used by the wifi standard in expressing bits and hex values,
		 * and it's also the convention used by wireshark in representing data and
		 * crc values. Note that this is different than the rest of our data representation,
		 * including this algorithm, where we want the oldest bit to occupy the msb in
		 * any given byte. Hence the following instruction sequence to convert from lsb-oldest
		 * to msb-oldest convention.
		 *
		 * Since we are only implementing big-endian, there is no endian-swap here. Byte-order
		 * convention with big-endian is that the MSB (most-significant-byte) is the oldest
		 * byte.
		 */
#if (CRC32_BITS_IN_BYTE_ORDER == lsb_OLDEST)
		InputWord = reverseBitsInBytes32(*pInputWords++);
#else
		InputWord = *pInputWords++;
#endif

		//        #endif
		Byte1  = _extu(InputWord, 0, 24);
		CrcValue   = ( CrcValue << 8 ) ^ CRCTable[ Byte1 ^ _extu(CrcValue, 0, 24) ];

		Byte2  = _extu(InputWord, 8, 24);
		CrcValue   = ( CrcValue << 8 ) ^ CRCTable[ Byte2 ^ _extu(CrcValue, 0, 24) ];

		Byte3  = _extu(InputWord, 16, 24);
		CrcValue   = ( CrcValue << 8 ) ^ CRCTable[ Byte3 ^ _extu(CrcValue, 0, 24) ];

		Byte4  = _extu(InputWord, 24, 24);
		CrcValue   = ( CrcValue << 8 ) ^ CRCTable[ Byte4 ^ _extu(CrcValue, 0, 24) ];
	}

	if ( NumNonWordBytes )
	{
#if (CRC32_BITS_IN_BYTE_ORDER == lsb_OLDEST)
		InputWord = reverseBitsInBytes32(*pInputWords++);
#else
		InputWord = *pInputWords++;
#endif

		Byte1  = _extu(InputWord, 0, 24);
		Byte2  = _extu(InputWord, 8, 24);
		Byte3  = _extu(InputWord, 16, 24);
		CrcValue   = ( CrcValue << 8 ) ^ CRCTable[ Byte1 ^ _extu(CrcValue, 0, 24) ];
		if ( NumNonWordBytes >= 2 )	{
			CrcValue   = ( CrcValue << 8 ) ^ CRCTable[ Byte2 ^ _extu(CrcValue, 0, 24) ];
		}
		if ( NumNonWordBytes == 3 )	{
			CrcValue   = ( CrcValue << 8 ) ^ CRCTable[ Byte3 ^ _extu(CrcValue, 0, 24) ];
		}
	}



//	CrcValue >>= ( 32 - NumCrcBits );
#if (N_BITS_IN_CRC < 32)
	CrcValue >>= (32 - N_BITS_IN_CRC);
#endif

	/* wifi-specific post-processing to crc to flip bits
	 * (one's complement).
	 */
	CrcValue ^= 0xFFFFFFFF;
	//    printf("CrcValue_pre: %08X\n", CrcValue);

	/* The same convention is used in returning the crc value, so that if the computed
	 * 32-bit crc is b32 b31.... b2 b1, it is converted to bytes as:
	 * crc = B1 B2 B3 B4 such that
	 * B1 = b25 b26 b27 b28 b29 b30 b31 b32,
	 * B2 = b17 ....................... b24,
	 * B3 = b9  ....................... b16,
	 * B4 = b1  ....................... b8.
	 */

#if (CRC32_BITS_IN_BYTE_ORDER == lsb_OLDEST)
	CrcValue = reverseBitsInBytes32(CrcValue);
#endif


	//	return(CrcValue);
	*arg_CrcValue = CrcValue;
}   /* end of WXLIB_crcCodecC64Long() function */




/**
 *  @fn     WXLIB_crcCodecC64Long
 *
 *  @desc   Computes a Crc (Cyclic Redundancy Checking) value
 *          for a given input buffer using 32-bit-wise implementation
 *          This function uses the table look-up method.

 *
 *  @arg    InputBytes[]
 *              input byte buffer
 *
 *  @arg    NumInputBytes
 *              number of input bytes
 *
 *  @arg    NumCrcBits
 *              number of CRC bit -- 16 bits or 32 bits
 *
 *  @arg    CRCTable[]
 *              look-up table
 *
 *  @return CRC
 *              return the CRC value for the input buffer
 *
 *  @enter
 *
 *  @leave
 *
 *  @see    WXLIB_crcCodecTableGenC64
 *
 */

/* must align InputBytes[] onto a 32 bit boundary */

//OUT Uint32 ORILIB_CRC32_fullStream(
//		IN Uint8 InputBytes[restrict],
//		IN Uint32 NumInputBytes,
//		IN Uint32 CRCTable[restrict]
//){
//
//	Uint32 CrcValue;
//
//	/* initialize CRC */
//	CrcValue = 0;
//
//	ORILIB_CRC32_block(
//			InputBytes,
//			NumInputBytes,
//			CRCTable,
//			&CrcValue
//	);
//
//	return CrcValue;
//}   /* end of WXLIB_crcCodecC64Long() function */


void ORILIB_CRC32_crcCodecTableGenC64(
		IN  Uint32    CrcPoly,
		OUT Uint32 *  CrcTablePtr 
		){
	Uint32 CrcByte;
	Uint32 Byte;
	Uint8  Bit;

	/* Compute the CRC of each possible byte.  */
	for ( Byte = 0; Byte < 256; Byte++ )
	{
		CrcByte = Byte << 24;

		/* Perform modulo-2 division, a bit at a time. */
		for ( Bit = 8; Bit > 0; --Bit )
		{
			/* Divide the current data bit. */
			if ( CrcByte & 0x80000000 )
				CrcByte = ( CrcByte << 1 ) ^ CrcPoly;
			else
				CrcByte = ( CrcByte << 1 );
		}

		/* Store next table entry. */
		CrcTablePtr[Byte] = CrcByte;
	}

	return;
}
