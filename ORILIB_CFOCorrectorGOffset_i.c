/**
Atomix project, ORILIB_CFOCorrectorGOffset_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "ORILIB_CFOCorrector_algos.h"

void ORILIB_CFOCorrectorGOffset_i(
		IN	void			*inBuf,
		IN	ORILIB_t_CFOState 	*cfoState,
		IN	ORILIB_t_PktAgcState 	*agcState, 
		OUT	void			*outBuf,
		CF	ORILIB_t_Offset2	*conf
		){

	Cplx16 *inSampleBuf 	= (Cplx16 *)((Uint8 *)inBuf + conf->inpByteOffset);
	Cplx16 *outSampleBuf 	= (Cplx16 *)((Uint8 *)outBuf + conf->outByteOffset);

	ORILIB_CFOCorrectorG_inner(
		inSampleBuf,
		cfoState,
		agcState,
		outSampleBuf
		);
}


void ORILIB_CFOCorrectorGOffset_i_conf(
		CF	ORILIB_t_Offset2	*conf,
		IN	Uint32			inpByteOffset,
		IN	Uint32			outByteOffset
		){
	conf->inpByteOffset = inpByteOffset;
	conf->outByteOffset = outByteOffset;
}
