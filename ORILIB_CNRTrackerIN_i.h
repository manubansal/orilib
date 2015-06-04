/**
Atomix project, ORILIB_CNRTrackerIN_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_CNRTRACKERIN_I_H 

#include "ORILIB_CNRTrackerIN_t.h"

void ORILIB_CNRTrackerIN_i(
  IN  ORILIB_t_Cplx16Buf80 const 	* const restrict unalignedRawSampleBuf,
  IN  ORILIB_t_CNRState * cnrState
  );


#endif /* ORILIB_CNRTRACKERIN_I_H */
