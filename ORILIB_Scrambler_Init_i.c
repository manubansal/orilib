/**
Atomix project, ORILIB_Scrambler_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_Descrambler_algorithms.h"

void ORILIB_Scrambler_Init_i (
  OUT ORILIB_t_Descrambler_State * outState
  ) {
  uint8_t first_byte = 0x6C; // init the scrambler with same sequence as matlab
  ORILIB_Descrambler_internal_pkt_init(first_byte, outState);
}
