/**
Atomix project, ORILIB_OFDMModulator_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_OFDMMODULATOR_T_H_
#define ORILIB_OFDMMODULATOR_T_H_

#include <osl/inc/swpform.h>
#include "ORILIB_t.h"
#include "ORILIB_OFDM_k.h"

typedef struct {
  uint8_t pilot_polarity_counter;
} ORILIB_OFDMModulator_t_State;

typedef struct {
  Uint32 scaleFactor;
} ORILIB_OFDMModulator_t_Conf;


#endif
