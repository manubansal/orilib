/**
Atomix project, ORILIB_CNRTracker_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_CNRTRACKER_I_H 

#include "ORILIB_CNRTracker_t.h"

void ORILIB_CNRTracker_i(
  IN  ORILIB_t_Cplx16Buf80 const 	* const restrict unalignedRawSampleBuf,
  OUT  ORILIB_t_CNRState * cnrState
  );


#endif /* ORILIB_CNRTRACKER_I_H */
