/**
Atomix project, ORILIB_ByteReader_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include <osl/inc/amem4cpy_inl.h>
#include "ORILIB_ByteReader_t.h"
#include "ORILIB_t.h"

#include <oros/sysilib/SYS_TimeStamp.h>

void ORILIB_ByteReader_i (
      IN  ORILIB_ByteReader_t_State * const inpStateBuf,
      OUT void * byteBuf,
      OUT ORILIB_ByteReader_t_State * outStateBuf,
      CF  ORILIB_ByteReader_t_Conf * conf
      ) {

  static far Uint32 nWraps = 0;
  Uint32 nextReadOffset = inpStateBuf->nextReadOffset; 

  //nBytes is the number of Cplx16 samples, that is, the number of Int16 pairs in the trace file
  if (nextReadOffset + conf->nBytesPerSymbol > (conf->nBytes)) {
    if (nWraps < conf->nWrapArounds) {
      nextReadOffset = 0;
      nWraps++;
    }
    else {
      LOG_PRINTF("ERROR: end of sample trace\n");
      SYS_TimeStampPrintAllAliased();
      exit(1);
    }
  }

  _amem4cpy(byteBuf, &(conf->bytes[nextReadOffset]), conf->nBytesPerSymbol >> 2); // >> 2 because size is in words
  nextReadOffset += conf->nBytesPerSymbol;
  outStateBuf->nextReadOffset = nextReadOffset;
}

void ORILIB_ByteReader_i_conf(
	CF  ORILIB_ByteReader_t_Conf * conf,
	Uint8 * bytes,
	Uint32 nBytesPerSymbol,
	Uint32 nBytes,
	Uint32 nWrapArounds
	){
  conf->bytes= bytes;
  conf->nBytesPerSymbol = nBytesPerSymbol;
  conf->nBytes = nBytes;
  conf->nWrapArounds = nWrapArounds;
}
