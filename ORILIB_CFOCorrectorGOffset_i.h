/**
Atomix project, ORILIB_CFOCorrectorGOffset_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_CFOCORRECTORGOFFSET_I_H 

#include "ORILIB_t.h"

void ORILIB_CFOCorrectorGOffset_i(
		IN	void			*inBuf,
		IN	ORILIB_t_CFOState 	*cfoState,
		IN	ORILIB_t_PktAgcState 	*agcState, 
		OUT	void			*outBuf,
		CF	ORILIB_t_Offset2	*conf
		);


void ORILIB_CFOCorrectorGOffset_i_conf(
		CF	ORILIB_t_Offset2	*conf,
		IN	Uint32			inpByteOffset,
		IN	Uint32			outByteOffset
		);

#endif /* ORILIB_CFOCORRECTORGOFFSET_I_H */
