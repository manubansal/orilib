/**
Atomix project, ORILIB_GPIO_input_i.c, Block on a GPIO pin until it takes on a value, or toggles
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_GPIO_input_t.h"
#include <osl/inc/swpform.h>
#include <ti/csl/csl_gpioAux.h>

void ORILIB_GPIO_input_i (
	CF  ORILIB_t_GPIOConf *conf
	){

	//Uint32 pinNum = 10;
	Uint32 pinNum = conf->pin;
	volatile Uint8 gpioData;
	volatile Uint8 entryState = 0;

	CSL_GpioHandle hGpio;

	//hGpio = CSL_GPIO_open (0);
	hGpio = CSL_GPIO_open(conf->bank);
	CSL_GPIO_setPinDirInput(hGpio, pinNum);

	CSL_GPIO_getInputData (hGpio, pinNum, &gpioData);
	entryState = gpioData;

	switch (conf->offOnToggle) {
	  case 0:  //wait until pin reads 0
	    while (gpioData != 0)
	      CSL_GPIO_getInputData (hGpio, pinNum, &gpioData);
	    break;

	  case 1:  //wait until pin reads 1
	    while (gpioData != 1)
	      CSL_GPIO_getInputData (hGpio, pinNum, &gpioData);
	    break;

	  default:  //wait until pin toggles
	    while (gpioData == entryState)
	      CSL_GPIO_getInputData (hGpio, pinNum, &gpioData);
	    break;
	}

	printf("entryState:%d,exitState:%d\n",entryState,gpioData);
}

void ORILIB_GPIO_input_i_conf (
	CF ORILIB_t_GPIOConf *conf,
	Uint8 bank,
	Uint8 pin,
	Uint8 offOnToggle
	){

	conf->bank = bank;
	conf->pin= pin;
	conf->offOnToggle = offOnToggle;
}
