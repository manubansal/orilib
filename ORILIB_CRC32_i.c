/**
Atomix project, ORILIB_CRC32_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include "WIFILIB_lookupTables.h"
#include "ORILIB_CRC32_t.h"
#include "ORILIB_CRC32_algos.h"

#ifdef SNRBER
#include <osl/OSL_SNRBER.h>
#endif

void ORILIB_CRC32_i (
	IN  void 			* inpBuf,
	IN  ORILIB_t_CRC32_State 	* inpState,
	OUT ORILIB_t_CRC32_State 	* outState,
	CF  ORILIB_t_CRC32_Conf		* conf
	){

  Uint32 crcValue = inpState->crcValue;

  ORILIB_CRC32_update (
      (Uint8 *)inpBuf,
      conf->nBytes,
      WIFILIB_crc32LookupTable,
      &crcValue
  );

#ifdef SNRBER_PKTDATAOUTDDR
  memcpy(PKTDATAOUT + PKTDATAOUT_nBytesWritten, inpBuf, conf->nBytes);
  PKTDATAOUT_nBytesWritten += conf->nBytes;
#endif

  outState->crcValue = crcValue;
  outState->nBytes = inpState->nBytes;
}

void ORILIB_CRC32_i_conf (
	CF ORILIB_t_CRC32_Conf * conf,
	IN Uint32 nBytes
	){

	conf->nBytes = nBytes;
}
