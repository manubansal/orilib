/**
Atomix project, ORILIB_ByteCacheGetFrame_a8_algo.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_BYTECACHEGETFRAME_A8_ALGO_H_

#include <osl/inc/swpform.h>
#include <osl/inc/amem8cpy_inl.h>
#include <osl/inc/amem4cpy_inl.h>
#include <osl/inc/mem4set_inl.h>

//inp cache contains: [..................................................]
//out frame contains: [.....................................]
//part decl consumed: [...........................]
//out cache contains:                              [.....................]
//out cache result  : [.....................]

static inline void ORILIB_ByteCacheGetFrame_a8_algo (
	IN  Int8 * restrict inpCacheBytes,
	OUT Int8 * restrict outBufBytes,
	OUT Int8 * restrict outCacheBytes,

	IN  Uint32 inpCacheValidLenInBytes,
	OUT Uint32 *outCacheValidLenInBytes,
	IN  Uint32 cacheCapacityInBytes,
	//IN  Uint32 outBufCapacityInBytes,

	IN  Uint32 outFrameSizeInBytes,
	IN  Uint32 consumeSizeInBytes,

	IN  Uint32 padSizeInWords,
	IN  Uint32 padWord
	) {


	//this allows retrieving *all* bytes in the cache by passing in the special values MAXUINT32
	outFrameSizeInBytes = outFrameSizeInBytes == MAXUINT32 ? inpCacheValidLenInBytes : outFrameSizeInBytes;
	consumeSizeInBytes = consumeSizeInBytes == MAXUINT32 ? inpCacheValidLenInBytes : consumeSizeInBytes;

	
	DEBUG(
	  ASSERT_PTR_ALIGNED(inpCacheBytes, 8);
	  ASSERT_PTR_ALIGNED(outBufBytes, 8);
	  ASSERT_PTR_ALIGNED(outCacheBytes, 8);

	  ASSERT(outFrameSizeInBytes <= inpCacheValidLenInBytes);
	  ASSERT(consumeSizeInBytes <= outFrameSizeInBytes);

	  //The following ensure that we can safely do transfers in
	  //multiples of 8 bytes, even if that extends beyond the 
	  //requested transfer lengths, without going out of memory
	  //bounds on the provided buffers.

	  //ASSERT(cacheCapacityInBytes % 8 == 0);
	  ASSERT(cacheCapacityInBytes % 4 == 0);

	  //ASSERT(outBufCapacityInBytes % 8 == 0);
	)

	//_amem8cpy(outBufBytes, inpCacheBytes, ceil(outFrameSizeInBytes, 8));
	_amem4cpy(outBufBytes, inpCacheBytes, ceil(outFrameSizeInBytes, 4));

	//_amem8cpy(&outCacheBytes[inpCacheValidLenInBytes], inpBufBytes, inpBufSizeInBytes/8);
	memcpy(	
		outCacheBytes, 
		&inpCacheBytes[consumeSizeInBytes],
		inpCacheValidLenInBytes - consumeSizeInBytes
		);


	//optional pad words
	_mem4set(&outBufBytes[outFrameSizeInBytes], padSizeInWords, padWord);

	*outCacheValidLenInBytes = 
		inpCacheValidLenInBytes - consumeSizeInBytes;
}


//new version: You can now set a cap on the total number of bytes that can 
//be consumed from the cache before the cache starts filling in pad bytes.


static inline void ORILIB_ByteCacheCappedGetFrame_a8_algo (
	IN  Int8 * restrict inpCacheBytes,
	OUT Int8 * restrict outBufBytes,
	OUT Int8 * restrict outCacheBytes,

	IN  Uint32 inpCacheValidLenInBytes,
	OUT Uint32 *outCacheValidLenInBytes,
	IN  Uint32 cacheCapacityInBytes,

	IN  Uint32 outFrameSizeInBytes,
	IN  Uint32 consumeSizeInBytes,

	IN  Uint32 padSizeInWords,
	IN  Uint32 padWord,

	IN  Uint32 totalConsumeCapInBytes,
	IN  Uint32 inpCacheTotalConsumedInBytes,
	OUT Uint32 *outCacheTotalConsumedInBytes
	) {


	Uint32 goodRemLength, goodCopyLength, padCopyLength, goodCopyLengthW,
	  padCopyLengthW, actualConsumeSizeInBytes;

	//**** PRE-ACCOUNTING *****
	//This allows retrieving *all* bytes in the cache by passing in the special values MAXUINT32
	//So effectively, this is conditionally modifying the "request"
	outFrameSizeInBytes = outFrameSizeInBytes == MAXUINT32 ? inpCacheValidLenInBytes : outFrameSizeInBytes;
	consumeSizeInBytes = consumeSizeInBytes == MAXUINT32 ? inpCacheValidLenInBytes : consumeSizeInBytes;

	//This computes how the request will be served - how many bytes to copy from cache, how many to pad
	goodRemLength = DIFF_NONNEG(totalConsumeCapInBytes, inpCacheTotalConsumedInBytes);
	//this length CAN be copied from the cache

	goodCopyLength = MIN(outFrameSizeInBytes, goodRemLength);	
	//this length WILL be copied from cache

	padCopyLength = outFrameSizeInBytes - goodCopyLength;		
	//this length will be copied from pad

	goodCopyLengthW = goodCopyLength/4;
	padCopyLengthW = padCopyLength/4;
	
	DEBUG(
	  ASSERT_PTR_ALIGNED(inpCacheBytes, 8);
	  ASSERT_PTR_ALIGNED(outBufBytes, 8);
	  ASSERT_PTR_ALIGNED(outCacheBytes, 8);

	  ASSERT(outFrameSizeInBytes <= inpCacheValidLenInBytes);
	  ASSERT(consumeSizeInBytes <= outFrameSizeInBytes);

	  //The following ensure that we can safely do transfers in
	  //multiples of 8 bytes, even if that extends beyond the 
	  //requested transfer lengths, without going out of memory
	  //bounds on the provided buffers.

	  //ASSERT(cacheCapacityInBytes % 8 == 0);
	  ASSERT(cacheCapacityInBytes % 4 == 0);

	  //ASSERT(outBufCapacityInBytes % 8 == 0);

	  ASSERT(goodCopyLength % 4 == 0);
	  ASSERT(padCopyLength % 4 == 0);
	)


	//**** OUTPUT-DATA-COPY *****
	//** copy good output
	//_amem8cpy(outBufBytes, inpCacheBytes, ceil(outFrameSizeInBytes, 8));
	//_amem4cpy(outBufBytes, inpCacheBytes, ceil(outFrameSizeInBytes, 4));
	_amem4cpy(outBufBytes, inpCacheBytes, goodCopyLengthW);

	//** copy pad words
	//_mem4set(&outBufBytes[outFrameSizeInBytes], padSizeInWords, padWord);
	_amem4set(&outBufBytes[goodCopyLength], padCopyLengthW, padWord);


	actualConsumeSizeInBytes = MIN(goodCopyLength, consumeSizeInBytes);


	//**** CACHE-UPDATE DATA-COPY *****
	//_amem8cpy(&outCacheBytes[inpCacheValidLenInBytes], inpBufBytes, inpBufSizeInBytes/8);
	memcpy(	
		outCacheBytes, 
		//&inpCacheBytes[consumeSizeInBytes],
		//inpCacheValidLenInBytes - consumeSizeInBytes
		&inpCacheBytes[actualConsumeSizeInBytes],
		inpCacheValidLenInBytes - actualConsumeSizeInBytes
		);

	//this one is a normalizing copy - this is so that we always pay the WCET for this block
	memcpy(
		&outCacheBytes[inpCacheValidLenInBytes - actualConsumeSizeInBytes],
		inpCacheBytes,
		cacheCapacityInBytes - (inpCacheValidLenInBytes - actualConsumeSizeInBytes)
		);


	//**** POST-ACCOUNTING *****
	*outCacheValidLenInBytes = 
		//inpCacheValidLenInBytes - consumeSizeInBytes;
		inpCacheValidLenInBytes - actualConsumeSizeInBytes;


	*outCacheTotalConsumedInBytes = inpCacheTotalConsumedInBytes + consumeSizeInBytes;
}

#endif

