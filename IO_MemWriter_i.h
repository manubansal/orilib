/**
Atomix project, IO_MemWriter_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define __IO_MEMWRITER_I_H__

#include <osl/inc/swpform.h>
#include "IO_MemWriter_t.h"

void IO_MemWriter_i (
	IN  void * byteBuf,
	IN  IO_t_MemWriter_State * const inpStateBuf,
	OUT IO_t_MemWriter_State * const outStateBuf,
	CF  IO_t_MemWriter_Conf * conf
	); 


void IO_MemWriter_i_conf(
	CF  IO_t_MemWriter_Conf * conf,
	Uint8 * bytes,
	Uint32 nBytesPerSymbol,
	Uint32 nBytes,
	Uint32 nWrapArounds
	);

#endif
