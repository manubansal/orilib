/**
Atomix project, ORILIB_ViterbiDecodingMap_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_VITERBIDECODINGMAP_I_H_
#define ORILIB_VITERBIDECODINGMAP_I_H_

#include "ORILIB_ViterbiDecodingMap_t.h"

void ORILIB_ViterbiDecodingMap_i (
	    IN  WIFILIB_TxRxPktState * const restrict inpPktState,
	    OUT Vitdec_ViterbiDecodingMap * const restrict vitdecMap,
	    CF  ORILIB_t_ViterbiDecodingMapConf *conf
	    );

void ORILIB_ViterbiDecodingMap_i_conf (
	    CF  ORILIB_t_ViterbiDecodingMapConf *conf,
	    IN  Uint32 frameLength,
	    IN  Uint32 convergenceLength
	    );

#endif

