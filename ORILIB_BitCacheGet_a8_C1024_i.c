/**
Atomix project, ORILIB_BitCacheGet_a8_C1024_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_BitCacheGet_a8_C1024_t.h"

void ORILIB_BitCacheGet_a8_C1024_i (
	IN  ORILIB_t_BitCache1024	* inpCache,
	OUT ORILIB_t_BitCache1024	* outCache,
	OUT void			* outBuf,
	CF  ORILIB_t_BitCacheGetConf * conf
	) {

	Int8 * inpCacheBits = inpCache->bits;

	//Int8 * outBufBits = outBuf->bits;
	Int8 * outBufBits = (Int8 *)outBuf;		//will work only if the 
							//out struct type is packed
	Int8 * outCacheBits = outCache->bits;

	Uint32 inpCacheValidLenInBits = inpCache->validLenInBits;
	Uint32 outCacheValidLenInBits;

	Uint32 cacheCapacityInBits = 1024;
	Uint32 outBufSizeInBits = conf->outBufSizeInBits;
	//Uint32 outBufCapacityInBits = conf->outBufCapacityInBits;
	Uint32 copySizeInBits = conf->copySizeInBits;


	ORILIB_BitCacheGet_a8_algo (
		inpCacheBits,
		outBufBits,
		outCacheBits,

		inpCacheValidLenInBits,
		&outCacheValidLenInBits,
		cacheCapacityInBits,
	  	//outBufCapacityInBits,

		outBufSizeInBits,
		copySizeInBits
	);

	outCache->validLenInBits = outCacheValidLenInBits;

}

void ORILIB_BitCacheGet_a8_C1024_i_conf (
	CF  ORILIB_t_BitCacheGetConf * conf,
	IN  Uint32 outBufSizeInBits,
	IN  Uint32 copySizeInBits
	){
	//IN  Uint32 outBufCapacityInBits,
	conf->outBufSizeInBits = outBufSizeInBits;
	//conf->outBufCapacityInBits = outBufCapacityInBits;
	conf->copySizeInBits = copySizeInBits;
}
