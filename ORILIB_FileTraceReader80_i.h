/**
Atomix project, ORILIB_FileTraceReader80_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_FILETRACEREADER80_I_H_
#define ORILIB_FILETRACEREADER80_I_H_

#include <osl/inc/swpform.h>
#include "ORILIB_FileTraceReader80_t.h"
#include <stdio.h>

void ORILIB_FileTraceReader80_i (
	OUT ORILIB_t_Cplx16Buf80 * const restrict sampleBuf,
	CF  ORILIB_FileTraceReader80_t_Conf * conf
	); 


void ORILIB_FileTraceReader80_i_conf(
	CF  ORILIB_FileTraceReader80_t_Conf * conf,
	char * traceFile
	);

#endif

