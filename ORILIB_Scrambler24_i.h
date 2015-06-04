/**
Atomix project, ORILIB_Scrambler24_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_SCRAMBLER24_I_H
#define ORILIB_SCRAMBLER24_I_H 

#include "ORILIB_Descrambler_t.h"

void ORILIB_Scrambler24_i (
	IN  ORILIB_t_BitBucket24            * inpBuf,
	IN  ORILIB_t_Descrambler_State      * inpStateBuf,
	OUT ORILIB_t_BitBucket24            * outBuf,
	OUT ORILIB_t_Descrambler_State      * outStateBuf
	);

#endif /* ORILIB_SCRAMBLER24_I_H */
