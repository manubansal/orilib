/**
Atomix project, ORILIB_BitCacheSkipPut_a8_C1024b_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_BitCache_a8_algos.h"
#include "ORILIB_BitCacheSkipPut_a8_C1024b_t.h"

void ORILIB_BitCacheSkipPut_a8_C1024b_i (
	IN  void			* inpBuf,
	IN  ORILIB_t_BitCache1024	* inpCache,
	OUT ORILIB_t_BitCache1024	* outCache,
	CF  ORILIB_t_BitCacheSkipPutConf * conf
	) {

	Int8 * inpCacheBits = inpCache->bits;

	Int8 * inpBufBits = (Int8 *)inpBuf;		//will work only if the 
							//out struct type is packed
	Int8 * outCacheBits = outCache->bits;

	Uint32 inpCacheValidLenInBits = inpCache->validLenInBits;
	Uint32 outCacheValidLenInBits;

	Uint32 cacheCapacityInBits = 1024;
	Uint32 inpBufSizeInBits = conf->inpBufSizeInBits;
	Uint32 inpCopySizeInBits = conf->inpCopySizeInBits;
	Uint32 inpSkipSizeInBits = conf->inpSkipSizeInBits;

	ORILIB_BitCacheSkipPut_a8_algo (
		inpCacheBits,
		inpBufBits,
		outCacheBits,

		inpCacheValidLenInBits,
		&outCacheValidLenInBits,

		cacheCapacityInBits,
	  	inpBufSizeInBits,

		inpCopySizeInBits,
		inpSkipSizeInBits
	);

	outCache->validLenInBits = outCacheValidLenInBits;

}

void ORILIB_BitCacheSkipPut_a8_C1024b_i_conf (
	CF  ORILIB_t_BitCacheSkipPutConf * conf,
	IN  Uint32 inpBufSizeInBits,
	IN  Uint32 inpCopySizeInBits,
	IN  Uint32 inpSkipSizeInBits
	){
	conf->inpBufSizeInBits = inpBufSizeInBits;
	conf->inpCopySizeInBits = inpCopySizeInBits;
	conf->inpSkipSizeInBits = inpSkipSizeInBits;
}
