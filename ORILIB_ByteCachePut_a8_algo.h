/**
Atomix project, ORILIB_ByteCachePut_a8_algo.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_BYTECACHEPUT_A8_ALGO_H_
#define ORILIB_BYTECACHEPUT_A8_ALGO_H_

#include <osl/inc/swpform.h>
#include <osl/inc/amem8cpy_inl.h>
#include <osl/inc/amem4cpy_inl.h>

static inline void ORILIB_ByteCachePut_a8_algo (
	IN  Int8 * restrict inpCacheBytes,
	IN  Int8 * restrict inpBufBytes,
	OUT Int8 * restrict outCacheBytes,

	IN  Uint32 inpCacheValidLenInBytes,
	OUT Uint32 *outCacheValidLenInBytes,

	IN  const Uint32 cacheCapacityInBytes,
	IN  const Uint32 inpBufSizeInBytes
	) {


	
	DEBUG(
	  ASSERT_PTR_ALIGNED(inpCacheBytes, 8);
	  ASSERT_PTR_ALIGNED(inpBufBytes, 8);
	  ASSERT_PTR_ALIGNED(outCacheBytes, 8);

	  //ASSERT(inpCacheValidLenInBytes % 8 == 0);
	  //ASSERT(cacheCapacityInBytes % 8 == 0);
	  //ASSERT(inpBufSizeInBytes % 8 == 0);

	  ASSERT(inpCacheValidLenInBytes % 4 == 0);
	  ASSERT(cacheCapacityInBytes % 4 == 0);

	  ASSERT(inpBufSizeInBytes % 4 == 0);

	  ASSERT(inpCacheValidLenInBytes + inpBufSizeInBytes <= cacheCapacityInBytes);
	)

	//_amem8cpy(outCacheBytes, inpCacheBytes, inpCacheValidLenInBytes/8);
	//_amem8cpy(&outCacheBytes[inpCacheValidLenInBytes], inpBufBytes, inpBufSizeInBytes/8);
	// TODO REMOVE POTENTIAL SOURCE OF VARIABILITY
	_amem4cpy(outCacheBytes, inpCacheBytes, inpCacheValidLenInBytes/4);
	_amem4cpy(&outCacheBytes[inpCacheValidLenInBytes], inpBufBytes, inpBufSizeInBytes/4);

	*outCacheValidLenInBytes = inpCacheValidLenInBytes + inpBufSizeInBytes;
	
}

#endif

