/**
Atomix project, ORILIB_ViterbiDecoder_GenConf_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include "ORILIB_ViterbiDecoder_t.h"
#include "ORILIB_ViterbiDecoder_GenConf_algo.h"

/** This function call requires the following parameters to be set previously:
 * state->data_rate. This parameter is actually a part of the initial configuration
 * of the corresponding atom. It is part of the "block definition/template", i.e.,
 * the static configuration, hence it would have already been set at system init.
 */
void ORILIB_ViterbiDecoder_GenConf_i (
	IN  Vitdec_ViterbiDecodingMap const * const restrict vitdecMap,
	IN  ORILIB_t_ViterbiDecoderState * const inpState,
	OUT ORILIB_t_ViterbiDecoderState * const outState,
	CF  ORILIB_t_ViterbiDecoderGenConfConf * const conf
	){

	//Int32 n4 = n2;									//no. of tail mode decodes (0 or 1) (tail-only, not head-tail)
	//Int32 l4 = c + l - f * (1 + n3);				//input frame length for tail decode
	//Int32 d4 = l4 - t;								//output frame length for tail decode
	//Int32 L4 = l4 - c - t;							//no. of useful bits obtained from tail decode
	//Int32 p4 = c;									//left strip length for tail decode
	//Int32 q4 = 0;									//right strip length for tail decode

	Uint32 num_bits_to_decode;
	switch (conf->mode) {
	  case VITDEC_HEADTAIL:
		  num_bits_to_decode = vitdecMap->d1;
		  break;

	  case VITDEC_HEADCONVGT:
		  num_bits_to_decode = vitdecMap->d2;
		  break;

	  case VITDEC_MIDCONVGT:
		  num_bits_to_decode = vitdecMap->d3;
		  break;

	  case VITDEC_TAILCONVGT:
		  num_bits_to_decode = vitdecMap->d4;
		  break;
	}

	ORILIB_ViterbiDecoder_GenConf_internal (
		num_bits_to_decode, 
		conf->mode,
		inpState,
		outState
		);

}


void ORILIB_ViterbiDecoder_GenConf_i_conf (
	CF  ORILIB_t_ViterbiDecoderGenConfConf * const conf,
	IN  Vitdec_Mode mode
	){
	conf->mode = mode;
}
