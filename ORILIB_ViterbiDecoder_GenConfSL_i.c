/**
Atomix project, ORILIB_ViterbiDecoder_GenConfSL_i.c, TODO: insert summary here
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
void ORILIB_ViterbiDecoder_GenConfSL_i (
	IN  ORILIB_t_ViterbiDecoderState * const inpState,
	OUT ORILIB_t_ViterbiDecoderState * const outState,
	CF  ORILIB_t_ViterbiDecoderGenConfConf * const conf
	){

	ORILIB_ViterbiDecoder_GenConf_internal (
		conf->num_bits_to_decode, 
		conf->mode,
		inpState,
		outState
		);

}

void ORILIB_ViterbiDecoder_GenConfSL_i_conf (
	CF  ORILIB_t_ViterbiDecoderGenConfConf * const conf,
	IN  Vitdec_Mode mode,
	IN  Uint32 num_bits_to_decode
	){
	conf->mode = mode;
	conf->num_bits_to_decode = num_bits_to_decode;
}
