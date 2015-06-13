/**
Atomix project, ORILIB_GPIO_input_i.h, Block on a GPIO pin until it takes on a value, or toggles
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_GPIO_INPUT_I_H
#define ORILIB_GPIO_INPUT_I_H 

#include "ORILIB_GPIO_input_t.h"
#include <osl/inc/swpform.h>
#include <ti/csl/csl_gpioAux.h>

void ORILIB_GPIO_input_i (
	CF  ORILIB_t_GPIOConf *conf
	);

void ORILIB_GPIO_input_i_conf (
	CF ORILIB_t_GPIOConf *conf,
	Uint8 bank,
	Uint8 pin,
	Uint8 offOnToggle
	);

#endif /* ORILIB_GPIO_INPUT_I_H */
