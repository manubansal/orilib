/**
Atomix project, ORILIB_Descrambler_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_DESCRAMBLER_I_H
#define ORILIB_DESCRAMBLER_I_H 

#include "ORILIB_Descrambler_t.h"

void ORILIB_Descrambler_i (
	IN  void				* const restrict inpBuf, 
	IN  ORILIB_t_Descrambler_State 		* restrict inpStateBuf, 
	OUT void				* const restrict outBuf, 
	OUT ORILIB_t_Descrambler_State 		* restrict outStateBuf,
	CF  ORILIB_t_DescramblerConf		* restrict conf
	);

void ORILIB_Descrambler_i_conf (
	CF  ORILIB_t_DescramblerConf * conf,
	IN  Uint32 data_length_in_bits
	);

#endif /* ORILIB_DESCRAMBLER_I_H */
