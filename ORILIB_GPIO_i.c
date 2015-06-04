/**
Atomix project, ORILIB_GPIO_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_GPIO_t.h"
#include <osl/inc/swpform.h>
#include <ti/csl/csl_gpioAux.h>

void ORILIB_GPIO_i (
	CF  ORILIB_t_GPIOConf *conf
	){
	CSL_GpioHandle gpio;
	Uint8 gpioData;

	gpio = CSL_GPIO_open(conf->bank);
	CSL_GPIO_setPinDirOutput(gpio, conf->pin);

	switch (conf->offOnToggle) {
		case 0:
			CSL_GPIO_clearOutputData(gpio, conf->pin);
		break;

		case 1:
			CSL_GPIO_setOutputData(gpio, conf->pin);
		break;

		default:
			CSL_GPIO_getOutputData(gpio, conf->pin, &gpioData);
			gpioData ^= 1;

			if (gpioData)
				CSL_GPIO_setOutputData(gpio, conf->pin);
			else
				CSL_GPIO_clearOutputData(gpio, conf->pin);
		break;
	}
}

void ORILIB_GPIO_i_conf (
	CF ORILIB_t_GPIOConf *conf,
	Uint8 bank,
	Uint8 pin,
	Uint8 offOnToggle
	){
	conf->bank = bank;
	conf->pin= pin;
	conf->offOnToggle = offOnToggle;
}
