/**
Atomix project, ORILIB_util_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_UTIL_I_H_
#define ORILIB_UTIL_I_H_

#include <osl/inc/swpform.h>

void printMacAddr(void * addr);

void ORILIB_gpio_output_control(
	Uint8 bank,
	Uint8 pin,
	Uint8 offOnToggle
	);

#endif
