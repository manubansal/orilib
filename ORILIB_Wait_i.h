/**
Atomix project, ORILIB_Wait_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_WAIT_I_H 

#include "ORILIB_Wait_t.h"

void ORILIB_Wait_i(
      CF  ORILIB_t_WaitConf * conf
      );

void ORILIB_Wait_i_conf(
      CF  ORILIB_t_WaitConf * conf,
      Uint64 nCycles
      );

#endif /* ORILIB_WAIT_I_H */
