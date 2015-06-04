/**
Atomix project, ORILIB_Softrate_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_SOFTRATE_T_H_ 
#define ORILIB_SOFTRATE_T_H_ 

#include "ORILIB_t.h"

#include <osl/inc/or_types.h> 
#include <orlib/orlib_blocks/orlib_types_wifi.h>
#include <orlib/orlib_fifos/inc/fifo.h>


typedef struct Softrate_MetricState
{
	/** Store the number of data bits per run, which is modulation-dependant */
	WifiGeeDataRates data_rate;
	/** Store the number of I bits per subcarrier, which is upper bounded by the number of bits
	 * per signal constellation symbol */	
//	uint16_t nIbpsc;
	
}Softrate_MetricState;

#endif