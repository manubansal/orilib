/**
Atomix project, ORILIB_DescramblerInit_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_DESCRAMBLERINIT_I_H
#define ORILIB_DESCRAMBLERINIT_I_H 

#include "ORILIB_Descrambler_t.h"


//init_byte_ptr:
//ptr to byte containing the first 7 bits of the scrambled sequence,
//first scrambled bit in msb position, that is, position 7 of 0:7,
//seventh scrambled bit in position 1 of 0:7 (0 is lsb, 7 is msb 
//in 0:7 notation). Since this is a void*, the ptr can point to any
//data structure that is one-byte or longer with the information in 
//first byte.
void ORILIB_DescramblerInit_i (
  IN  void * const init_byte_ptr,	
  OUT ORILIB_t_Descrambler_State * const restrict outState
  );

#endif /* ORILIB_DESCRAMBLERINIT_I_H */
