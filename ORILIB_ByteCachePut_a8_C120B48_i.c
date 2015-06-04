/**
Atomix project, ORILIB_ByteCachePut_a8_C120B48_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include "ORILIB_ByteCachePut_a8_algo.h"
#include "ORILIB_ByteCachePut_a8_C120B48_t.h"

void ORILIB_ByteCachePut_a8_C120B48_i (
	IN  void			* inpBuf,
	IN  ORILIB_t_ByteCache120	* inpCache,
	OUT ORILIB_t_ByteCache120	* outCache
	) {

	Int8 * inpCacheBytes = inpCache->bytes;

	//Int8 * inpBufBytes = inpBuf->bytes;
	Int8 * inpBufBytes = (Int8 *)inpBuf;		//will work only if the 
							//input struct type is packed
	Int8 * outCacheBytes = outCache->bytes;

	Uint32 inpCacheValidLenInBytes = inpCache->validLenInBytes;
	Uint32 outCacheValidLenBytes;

	Uint32 cacheCapacityInBytes = 120;
	Uint32 inpBufSizeInBytes = 48;


	ORILIB_ByteCachePut_a8_algo (
		inpCacheBytes,
		inpBufBytes,
		outCacheBytes,

		inpCacheValidLenInBytes,
		&outCacheValidLenBytes,

		cacheCapacityInBytes,
		inpBufSizeInBytes
	);

	outCache->validLenInBytes = outCacheValidLenBytes;

}
