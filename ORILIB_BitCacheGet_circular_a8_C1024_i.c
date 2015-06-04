/**
Atomix project, ORILIB_BitCacheGet_circular_a8_C1024_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_BitCache_a8_algos.h"
#include "ORILIB_BitCacheGet_a8_C1024_t.h"

void ORILIB_BitCacheGet_circular_a8_C1024_i (
	IN  ORILIB_t_BitCache1024_circular	* inpCache,
	OUT void				* outBuf,
	CF  ORILIB_t_BitCacheGetConf 		* conf
	){

	Int8 * inpCacheBits = inpCache->bits;

	Int8 * outBufBits = (Int8 *)outBuf;		//will work only if the 
							//out struct type is packed

	Uint32 inpCacheValidLenInBits = inpCache->validLenInBits;
	Uint32 inpCacheValidBitsOffset = inpCache->validBitsOffset;
	Uint32 outCacheValidLenInBits;
	Uint32 outCacheValidBitsOffset;

	Uint32 cacheCapacityInBits = 1024;
	Uint32 outBufSizeInBits = conf->outBufSizeInBits;
	Uint32 copySizeInBits = conf->copySizeInBits;


	ORILIB_BitCacheGet_circular_a8_algo (
		inpCacheBits,
		outBufBits,

		inpCacheValidLenInBits,
		inpCacheValidBitsOffset,
		&outCacheValidLenInBits,
		&outCacheValidBitsOffset,
		cacheCapacityInBits,

		outBufSizeInBits,
		copySizeInBits
	);

	inpCache->validLenInBits = outCacheValidLenInBits;
	inpCache->validBitsOffset = outCacheValidBitsOffset;

}

void ORILIB_BitCacheGet_circular_a8_C1024_i_conf (
	CF  ORILIB_t_BitCacheGetConf * conf,
	IN  Uint32 outBufSizeInBits,
	IN  Uint32 copySizeInBits
	){
	conf->outBufSizeInBits = outBufSizeInBits;
	conf->copySizeInBits = copySizeInBits;
}
