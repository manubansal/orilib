/**
Atomix project, ORILIB_GPIO_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_GPIO_I_H_
#define ORILIB_GPIO_I_H_

void ORILIB_GPIO_i (
	CF  ORILIB_t_GPIOConf *conf
	);

void ORILIB_GPIO_i_conf (
	CF ORILIB_t_GPIOConf *conf,
	Uint8 bank,
	Uint8 pin,
	Uint8 offOnToggle
	);

#endif
