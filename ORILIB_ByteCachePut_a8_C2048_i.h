/**
Atomix project, ORILIB_ByteCachePut_a8_C2048_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_BYTECACHEPUT_A8_C2048_I_H_
#define ORILIB_BYTECACHEPUT_A8_C2048_I_H_

#include "ORILIB_ByteCachePut_a8_algo.h"
#include "ORILIB_ByteCachePut_a8_C2048_t.h"

void ORILIB_ByteCachePut_a8_C2048_i (
	IN  void			* inpBuf,
	IN  ORILIB_t_ByteCache2048	* inpCache,
	OUT ORILIB_t_ByteCache2048	* outCache,
	CF  ORILIB_t_ByteCache_Conf	* conf
	);


void ORILIB_ByteCachePut_a8_C2048_i_conf (
	CF  ORILIB_t_ByteCache_Conf	* conf,
	IN  Uint32 copySizeInBytes
	);


#endif

