/**
Atomix project, ORILIB_GPIO_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_GPIO_T_H_

#include <osl/inc/swpform.h>
#include "ORILIB_GPIO_t.h"

typedef struct {
	Uint8 bank;
	Uint8 pin;
	Uint8 offOnToggle;
} ORILIB_t_GPIOConf;

#endif
