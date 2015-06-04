/**
Atomix project, ORILIB_ByteCacheGetFrame_a8_C2048_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_ByteCacheGetFrame_a8_algo.h"
#include "ORILIB_ByteCacheGetFrame_a8_C2048_t.h"

void ORILIB_ByteCacheGetFrame_a8_C2048_i (
	IN  ORILIB_t_ByteCache2048	* inpCache,
	OUT ORILIB_t_ByteCache2048	* outCache,
	OUT void			* outBuf,
	CF  ORILIB_t_ByteCacheGetFrameConf * conf
	) {

	Int8 * inpCacheBytes = inpCache->bytes;

	//Int8 * outBufBytes = outBuf->bytes;
	Int8 * outBufBytes = (Int8 *)outBuf;		//will work only if the 
							//out struct type is packed
	Int8 * outCacheBytes = outCache->bytes;

	Uint32 inpCacheValidLenInBytes = inpCache->validLenInBytes;
	Uint32 outCacheValidLenInBytes;

	Uint32 cacheCapacityInBytes = 2048;
	Uint32 outFrameSizeInBytes = conf->outFrameSizeInBytes;
	//Uint32 outBufCapacityInBytes = conf->outBufCapacityInBytes;
	Uint32 consumeSizeInBytes = conf->consumeSizeInBytes;


	ORILIB_ByteCacheGetFrame_a8_algo (
		inpCacheBytes,
		outBufBytes,
		outCacheBytes,

		inpCacheValidLenInBytes,
		&outCacheValidLenInBytes,
		cacheCapacityInBytes,
	  	//outBufCapacityInBytes,

		outFrameSizeInBytes,
		consumeSizeInBytes,

		conf->padSizeInWords,
		conf->padWord
	);

	outCache->validLenInBytes = outCacheValidLenInBytes;

}

//inp cache contains: [..................................................]
//out frame contains: [.....................................]
//part decl consumed: [...........................]
//out cache contains:                              [.....................]
//out cache result  : [.....................]
void ORILIB_ByteCacheGetFrame_a8_C2048_i_conf (
	CF  ORILIB_t_ByteCacheGetFrameConf * conf,
	IN  Uint32 outFrameSizeInBytes,
	IN  Uint32 consumeSizeInBytes,
	IN  Uint32 padSizeInWords,
	IN  Uint32 padWord
	){
	//IN  Uint32 outBufCapacityInBytes,
	conf->outFrameSizeInBytes = outFrameSizeInBytes;
	//conf->outBufCapacityInBytes = outBufCapacityInBytes;
	conf->consumeSizeInBytes = consumeSizeInBytes;
	conf->padSizeInWords = padSizeInWords;
	conf->padWord = padWord;
}
