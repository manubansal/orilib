/**
Atomix project, ORILIB_CRC32_VarBytes_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_CRC32_VARBYTES_I_H 

#include "ORILIB_CRC32_VarBytes_t.h"

void ORILIB_CRC32_VarBytes_i (
	IN  void 			* inpBuf,
	IN  ORILIB_t_CRC32_State 	* inpState,
	OUT ORILIB_t_CRC32_State 	* outState
	);

#endif /* ORILIB_CRC32_VARBYTES_I_H */
