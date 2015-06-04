/**
Atomix project, ORILIB_ViterbiDecodingMap_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

void ORILIB_ViterbiDecodingMap_i (
	    IN  WIFILIB_TxRxPktState * const restrict inpPktState,
	    OUT Vitdec_ViterbiDecodingMap * const restrict vitdecMap,
	    CF  ORILIB_t_ViterbiDecodingMapConf *conf
	    ){

	//Uint32 frameLength = 24; 		//for 6mbps
	//Uint32 frameLength = 216/4 = 54; 	//for 54mbps 4-ways
	Uint32 frameLength = conf->frameLength;
	Uint32 convergenceLength = conf->convergenceLength;
	vitdec_wifiGee_setViterbiDecodingMap(
		inpPktState->NumInfoBits,
		inpPktState->NumDataBitsPerSymbol,
		frameLength, 
		convergenceLength,
		vitdecMap
		);

}

void ORILIB_ViterbiDecodingMap_i_conf (
	    CF  ORILIB_t_ViterbiDecodingMapConf *conf,
	    IN  Uint32 frameLength,
	    IN  Uint32 convergenceLength
	    ){
	    conf->frameLength = frameLength;
	    conf->convergenceLength = convergenceLength;
}
