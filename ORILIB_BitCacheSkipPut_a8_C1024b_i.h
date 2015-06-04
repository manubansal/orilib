/**
Atomix project, ORILIB_BitCacheSkipPut_a8_C1024b_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_BITCACHESKIPPUT_A8_C1024B_I_H_

void ORILIB_BitCacheSkipPut_a8_C1024b_i (
	IN  void			* inpBuf,
	IN  ORILIB_t_BitCache1024	* inpCache,
	OUT ORILIB_t_BitCache1024	* outCache,
	CF  ORILIB_t_BitCacheSkipPutConf * conf
	);

void ORILIB_BitCacheSkipPut_a8_C1024b_i_conf (
	CF  ORILIB_t_BitCacheSkipPutConf * conf,
	IN  Uint32 inpBufSizeInBits,
	IN  Uint32 inpCopySizeInBits,
	IN  Uint32 inpSkipSizeInBits
	);

#endif

