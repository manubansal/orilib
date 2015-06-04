/**
Atomix project, ORILIB_BufferJoin2To1_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_BUFFERJOIN2TO1_I_H_

#include <osl/inc/swpform.h>
#include "ORILIB_BufferJoin2To1_t.h"

void ORILIB_BufferJoin2To1_i (
	IN  void *bufInp1,
	IN  void *bufInp2,
	OUT void *bufOut,
	CF  ORILIB_BufferJoin2To1_tConf *conf
	);

void ORILIB_BufferJoin2To1_i_conf (
	CF  ORILIB_BufferJoin2To1_tConf *conf,
	IN  Uint32 len_in_bytes
	);

#endif

