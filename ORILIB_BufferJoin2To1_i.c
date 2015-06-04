/**
Atomix project, ORILIB_BufferJoin2To1_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_BufferJoin2To1_t.h"
#include <osl/inc/amem4cpy_inl.h>

void ORILIB_BufferJoin2To1_i (
	IN  void *bufInp1,
	IN  void *bufInp2,
	OUT void *bufOut,
	CF  ORILIB_BufferJoin2To1_tConf *conf
	) {

  Uint32 len_in_bytes;
  len_in_bytes = conf->len;

  _amem4cpy((Uint32 *)bufOut, (Uint32 *)bufInp1, len_in_bytes/4);
  _amem4cpy(&((Uint32 *)bufOut)[len_in_bytes/4], (Uint32 *)bufInp2, len_in_bytes/4);
}

void ORILIB_BufferJoin2To1_i_conf (
	CF  ORILIB_BufferJoin2To1_tConf *conf,
	IN  Uint32 len_in_bytes
	) {
  conf->len = len_in_bytes;
}
