/**
Atomix project, IO_MemWriter_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
Atomix project, IO_MemWriter_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/amem4cpy_inl.h>
#include "IO_MemWriter_t.h"
#include "ORILIB_t.h"

#include <oros/sysilib/SYS_TimeStamp.h>

void IO_MemWriter_i (
      IN  void * byteBuf,
      IN  IO_t_MemWriter_State * const inpStateBuf,
      OUT IO_t_MemWriter_State * outStateBuf,
      CF  IO_t_MemWriter_Conf * conf
      ) {

  static far Uint32 nWraps = 0;
  Uint32 nextWriteOffset = inpStateBuf->nextWriteOffset; 

  //nBytes is the number of Cplx16 samples, that is, the number of Int16 pairs in the trace file
  if (nextWriteOffset + conf->nBytesPerSymbol > (conf->nBytes)) {
    if (nWraps < conf->nWrapArounds) {
      nextWriteOffset = 0;
      nWraps++;
    }
    else {
      LOG_PRINTF("ERROR: end of wrap-around limit, cannot write more\n");
      SYS_TimeStampPrintAllAliased();
      exit(1);
    }
  }

  _amem4cpy(&(conf->bytes[nextWriteOffset]), byteBuf, conf->nBytesPerSymbol >> 2); // >> 2 because size is in words
  nextWriteOffset += conf->nBytesPerSymbol;
  outStateBuf->nextWriteOffset = nextWriteOffset;
}

void IO_MemWriter_i_conf(
	CF  IO_t_MemWriter_Conf * conf,
	Uint8 * bytes,
	Uint32 nBytesPerSymbol,
	Uint32 nBytes,
	Uint32 nWrapArounds
	){

  assert(nBytesPerSymbol % 4 == 0);	//buffer size to write each time is a mutiple of 4 bytes
  assert((Uint32)bytes % 4 == 0);		//pointer is aligned to 4-byte boundary

  conf->bytes= bytes;
  conf->nBytesPerSymbol = nBytesPerSymbol;
  conf->nBytes = nBytes;
  conf->nWrapArounds = nWrapArounds;
}
