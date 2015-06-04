/**
Atomix project, ORILIB_CRC32_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_CRC32_I_H
#define ORILIB_CRC32_I_H 

#include "ORILIB_CRC32_t.h"

void ORILIB_CRC32_i (
	IN  void 			* inpBuf,
	IN  ORILIB_t_CRC32_State 	* inpState,
	OUT ORILIB_t_CRC32_State 	* outState,
	CF  ORILIB_t_CRC32_Conf		* conf
	);

void ORILIB_CRC32_i_conf (
	CF ORILIB_t_CRC32_Conf * conf,
	IN Uint32 nBytes
	);


#endif /* ORILIB_CRC32_I_H */
