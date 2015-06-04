/**
Atomix project, ORILIB_Descrambler288_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_DESCRAMBLER288_I_H 

#include "ORILIB_Descrambler_t.h"

void ORILIB_Descrambler288_i (
	IN  ORILIB_t_BitBucket288 		* const restrict inpBuf, 
	IN  ORILIB_t_Descrambler_State 		* restrict inpStateBuf, 
	OUT ORILIB_t_BitBucket288 		* const restrict outBuf, 
	OUT ORILIB_t_Descrambler_State 		* restrict outStateBuf
	);

#endif /* ORILIB_DESCRAMBLER288_I_H */
