/**
Atomix project, ORILIB_BitCache_a8_algos.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_BITCACHE_A8_ALGOS_H_
#define ORILIB_BITCACHE_A8_ALGOS_H_

#include <osl/inc/swpform.h>
#include <osl/inc/amem4bitcpy.h>

//inp cache contains: [aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.............]
//inp buf   contains: [bbbbbbbbcccccccccccccddddddddd.......]
//out cache contains: [a......ac......c.....]
//where b's correspond to the skipped bits from the input,
//c's fall in the copy length after the skip length,
//and d's fall out of the copy length after the skip length.

static inline void ORILIB_BitCacheSkipPut_a8_algo (
	IN  Int8 * restrict inpCacheBits,
	IN  Int8 * restrict inpBufBits,
	OUT Int8 * restrict outCacheBits,

	IN  Uint32 inpCacheValidLenInBits,
	OUT Uint32 *outCacheValidLenInBits,

	IN  Uint32 cacheCapacityInBits,
	IN  Uint32 inpBufSizeInBits,
	
	IN  Uint32 inpCopySizeInBits,
	IN  Uint32 inpSkipSizeInBits
	) {


	DEBUG(
	  ASSERT_PTR_ALIGNED(inpCacheBits, 8);
	  ASSERT_PTR_ALIGNED(inpBufBits, 8);
	  ASSERT_PTR_ALIGNED(outCacheBits, 8);

	  ASSERT(cacheCapacityInBits % (4 * 8) == 0);
	  ASSERT(inpBufSizeInBits % (4 * 8) == 0);

	  ASSERT(inpSkipSizeInBits + inpCopySizeInBits <= inpBufSizeInBits);
	  ASSERT(inpCacheValidLenInBits + inpCopySizeInBits <= cacheCapacityInBits);
	)


//static inline void amem4bitcpy(
//		OUT	Uint32 *outBuf,
//		IN	Uint32 *inBuf,
//		IN	Uint32 nBitsToCopy,
//		IN	Uint32 nInpBitsToSkip
//		) {
	amem4bitcpy((Uint32 *)outCacheBits, (Uint32 *)inpCacheBits, inpCacheValidLenInBits, 0);

//static inline void amem4bitcpy_outskip(
//		OUT	Uint32 *outBuf,
//		IN	Uint32 *inBuf,
//		IN	Uint32 nBitsToCopy,
//		IN	Uint32 nOutBitsToSkip,
//		IN	Uint32 nInpBitsToSkip
//		) {
	amem4bitcpy_outskip((Uint32 *)outCacheBits, (Uint32 *)inpBufBits, inpCopySizeInBits, inpCacheValidLenInBits, inpSkipSizeInBits);

	*outCacheValidLenInBits = inpCacheValidLenInBits + inpCopySizeInBits;

}


//inp cache contains: [x...xa......ay...........y], where x and y are don't-care data
//inp buf   contains: [bbbbbbbbcccccccccccccddddddddd.......]
//after operation:
//inp cache contains: [x...xa......ac......c.....], if no wraparound occured, or
//inp cache contains: [cc..xa......ac......c.....], if wraparound occured
//where b's correspond to the skipped bits from the input,
//c's fall in the copy length after the skip length,
//and d's fall out of the copy length after the skip length.

static inline void ORILIB_BitCacheSkipPut_circular_a8_algo (
	IN  Int8 * restrict inpCacheBits,
	IN  Int8 const * const restrict inpBufBits,

	IN  Uint32 inpCacheValidLenInBits,
	IN  Uint32 inpCacheValidBitsOffset,		//start offset of valid data, in bits
	OUT Uint32 *outCacheValidLenInBits,

	IN  Uint32 cacheCapacityInBits,
	IN  Uint32 inpBufSizeInBits,
	
	IN  Uint32 inpCopySizeInBits,
	IN  Uint32 inpSkipSizeInBits
	) {


	DEBUG(
	  ASSERT_PTR_ALIGNED(inpCacheBits, 8);
	  ASSERT_PTR_ALIGNED(inpBufBits, 8);
	  ASSERT_PTR_ALIGNED(outCacheBits, 8);

	  ASSERT(cacheCapacityInBits % (4 * 8) == 0);
	  ASSERT(inpBufSizeInBits % (4 * 8) == 0);

	  ASSERT(inpSkipSizeInBits + inpCopySizeInBits <= inpBufSizeInBits);
	  ASSERT(inpCacheValidLenInBits + inpCopySizeInBits <= cacheCapacityInBits);
	)


	Uint32 firstCopyStartOffset = (inpCacheValidBitsOffset + inpCacheValidLenInBits) % cacheCapacityInBits;
	Uint32 doWeWrapAround = (firstCopyStartOffset + inpCopySizeInBits) > cacheCapacityInBits;
	Int32 nBitsToCopyAfterWraparound = doWeWrapAround ? (firstCopyStartOffset + inpCopySizeInBits) % cacheCapacityInBits : 0;
	Int32 nBitsToCopyBeforeWraparound = inpCopySizeInBits - nBitsToCopyAfterWraparound;

	//Int32 nBitsToCopyBeforeWraparound =
	//  MIN(
	//    MAX(((Int32)cacheCapacityInBits - (Int32)inpCacheValidBitsOffset - (Int32)inpCacheValidLenInBits), 0),
	//      inpCopySizeInBits);




//static inline void amem4bitcpy_outskip(
//		OUT	Uint32 *outBuf,
//		IN	Uint32 *inBuf,
//		IN	Uint32 nBitsToCopy,
//		IN	Uint32 nOutBitsToSkip,
//		IN	Uint32 nInpBitsToSkip
//		) {
	amem4bitcpy_outskip((Uint32 *)inpCacheBits, (Uint32 *)inpBufBits, nBitsToCopyBeforeWraparound, firstCopyStartOffset, inpSkipSizeInBits);

	amem4bitcpy_outskip((Uint32 *)inpCacheBits, (Uint32 *)inpBufBits, nBitsToCopyAfterWraparound, 0, inpSkipSizeInBits + nBitsToCopyBeforeWraparound);

	*outCacheValidLenInBits = inpCacheValidLenInBits + inpCopySizeInBits;
}

static inline void ORILIB_BitCacheGet_a8_algo (
	IN  Int8 * restrict inpCacheBits,
	IN  Int8 * restrict outBufBits,
	OUT Int8 * restrict outCacheBits,

	IN  Uint32 inpCacheValidLenInBits,
	OUT Uint32 *outCacheValidLenInBits,
	IN  Uint32 cacheCapacityInBits,

	IN  Uint32 outBufSizeInBits,
	IN  Uint32 copySizeInBits
	) {


	DEBUG(
	  ASSERT_PTR_ALIGNED(inpCacheBits, 8);
	  ASSERT_PTR_ALIGNED(outBufBits, 8);
	  ASSERT_PTR_ALIGNED(outCacheBits, 8);

	  ASSERT(cacheCapacityInBits % (4 * 8) == 0);
	  ASSERT(outBufSizeInBits % (4 * 8) == 0);

	  ASSERT(copySizeInBits <= outBufSizeInBits);
	  ASSERT(copySizeInBits <= inpCacheValidLenInBits);
	)


	amem4bitcpy((Uint32 *)outBufBits, (Uint32 *)inpCacheBits, copySizeInBits, 0);
	amem4bitcpy((Uint32 *)outCacheBits, (Uint32 *)inpCacheBits, inpCacheValidLenInBits - copySizeInBits, copySizeInBits);

	*outCacheValidLenInBits = inpCacheValidLenInBits - copySizeInBits;

}

static inline void ORILIB_BitCacheGet_circular_a8_algo (
	IN  Int8 const * const restrict inpCacheBits,
	IN  Int8 * const restrict outBufBits,

	IN  Uint32 const inpCacheValidLenInBits,
	IN  Uint32 const inpCacheValidBitsOffset,
	OUT Uint32 * const restrict outCacheValidLenInBits,
	OUT Uint32 * const restrict outCacheValidBitsOffset,
	IN  Uint32 const cacheCapacityInBits,

	IN  Uint32 const outBufSizeInBits,
	IN  Uint32 const copySizeInBits
	) {

	Int32 nBitsToCopyBeforeWraparound = 
	  MIN((Int32)cacheCapacityInBits - (Int32)inpCacheValidBitsOffset, copySizeInBits);

	Int32 nBitsToCopyAfterWraparound = copySizeInBits - nBitsToCopyBeforeWraparound;

	DEBUG(
	  ASSERT_PTR_ALIGNED(inpCacheBits, 8);
	  ASSERT_PTR_ALIGNED(outBufBits, 8);
	  ASSERT_PTR_ALIGNED(outCacheBits, 8);

	  ASSERT(cacheCapacityInBits % (4 * 8) == 0);
	  ASSERT(outBufSizeInBits % (4 * 8) == 0);

	  ASSERT(copySizeInBits <= outBufSizeInBits);
	  ASSERT(copySizeInBits <= inpCacheValidLenInBits);
	)


//static inline void amem4bitcpy_outskip(
//		OUT	Uint32 *outBuf,
//		IN	Uint32 *inBuf,
//		IN	Uint32 nBitsToCopy,
//		IN	Uint32 nOutBitsToSkip,
//		IN	Uint32 nInpBitsToSkip
//		) {
	amem4bitcpy_outskip((Uint32 *)outBufBits, (Uint32 *)inpCacheBits, nBitsToCopyBeforeWraparound, 0, inpCacheValidBitsOffset);

	amem4bitcpy_outskip((Uint32 *)outBufBits, (Uint32 *)inpCacheBits, nBitsToCopyAfterWraparound, nBitsToCopyBeforeWraparound, 0);

	*outCacheValidLenInBits = inpCacheValidLenInBits - copySizeInBits;
	*outCacheValidBitsOffset = nBitsToCopyAfterWraparound ? nBitsToCopyAfterWraparound :
		inpCacheValidBitsOffset + nBitsToCopyBeforeWraparound;

}


#endif

