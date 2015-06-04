/**
Atomix project, ORILIB_CRC32_algos.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_CRC32_ALGOS_H 

#include <osl/inc/swpform.h>
#include <osl/inc/util.h>


void ORILIB_CRC32_update (
		IN Uint8 InputBytes[restrict],
		IN Uint32 NumInputBytes,
		IN Uint32 CRCTable[restrict],
		INOUT	Uint32 *arg_CrcValue
		);


//Uint32 ORILIB_CRC32_fullStream( IN Uint8 InputBytes[restrict],
//              IN Uint32 NumInputBytes,
//              IN Uint32 CRCTable[restrict]
//		);

void ORILIB_CRC32_crcCodecTableGenC64(
		IN  Uint32    CrcPoly,
		OUT Uint32 *  CrcTablePtr 
		);

#endif /* ORILIB_CRC32_ALGOS_H */
