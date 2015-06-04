/**
Atomix project, ORILIB_ByteReader_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef __ORILIB_BYTEREADER_I_H__
#define __ORILIB_BYTEREADER_I_H__

#include <osl/inc/swpform.h>
#include "ORILIB_ByteReader_t.h"

void ORILIB_ByteReader_i (
	IN  ORILIB_ByteReader_t_State * const inpStateBuf,
	OUT void * byteBuf,
	OUT ORILIB_ByteReader_t_State * const outStateBuf,
	CF  ORILIB_ByteReader_t_Conf * conf
	); 


void ORILIB_ByteReader_i_conf(
	CF  ORILIB_ByteReader_t_Conf * conf,
	Uint8 * bytes,
	Uint32 nBytesPerSymbol,
	Uint32 nBytes,
	Uint32 nWrapArounds
	);

#endif
