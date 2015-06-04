/**
Atomix project, ORILIB_CRC32_VarBytes_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include "WIFILIB_lookupTables.h"
#include "ORILIB_CRC32_VarBytes_t.h"
#include "ORILIB_CRC32_algos.h"

#ifdef SNRBER
#include <osl/OSL_SNRBER.h>
#endif

void ORILIB_CRC32_VarBytes_i (
	IN  void 			* inpBuf,
	IN  ORILIB_t_CRC32_State 	* inpState,
	OUT ORILIB_t_CRC32_State 	* outState
	){

  Uint32 crcValue = inpState->crcValue;
  Uint32 nBytes = inpState->nBytes;

  ORILIB_CRC32_update (
      (Uint8 *)inpBuf,
      nBytes,
      WIFILIB_crc32LookupTable,
      &crcValue
  );

#ifdef SNRBER_PKTDATAOUTDDR
  memcpy(PKTDATAOUT + PKTDATAOUT_nBytesWritten, inpBuf, nBytes);
  PKTDATAOUT_nBytesWritten += nBytes;
#endif

  outState->crcValue = crcValue;
  outState->nBytes = nBytes;
}
