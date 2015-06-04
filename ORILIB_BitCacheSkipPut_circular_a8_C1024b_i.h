/**
Atomix project, ORILIB_BitCacheSkipPut_circular_a8_C1024b_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_BITCACHESKIPPUT_CIRCULAR_A8_C1024B_I_H 

#include "ORILIB_t.h"

void ORILIB_BitCacheSkipPut_circular_a8_C1024b_i (
	IN  void				* inpBuf,
	IN  ORILIB_t_BitCache1024_circular	* inpCache,
	CF  ORILIB_t_BitCacheSkipPutConf 	* conf
	);

void ORILIB_BitCacheSkipPut_circular_a8_C1024b_i_conf (
	CF  ORILIB_t_BitCacheSkipPutConf * conf,
	IN  Uint32 inpBufSizeInBits,
	IN  Uint32 inpCopySizeInBits,
	IN  Uint32 inpSkipSizeInBits
	);

#endif /* ORILIB_BITCACHESKIPPUT_CIRCULAR_A8_C1024B_I_H */
