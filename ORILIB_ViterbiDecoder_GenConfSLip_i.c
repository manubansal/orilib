/**
Atomix project, ORILIB_ViterbiDecoder_GenConfSLip_i.c, TODO: insert summary here
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
//state is inout state
void ORILIB_ViterbiDecoder_GenConfSLip_i (
	IN  ORILIB_t_ViterbiDecoderState * const inoutState,
	CF  ORILIB_t_ViterbiDecoderGenConfConf * const conf
	){

	ORILIB_ViterbiDecoder_GenConfip_internal (
		conf->num_bits_to_decode, 
		conf->mode,
		inoutState
		);

}

void ORILIB_ViterbiDecoder_GenConfSLip_i_conf (
	CF  ORILIB_t_ViterbiDecoderGenConfConf * const conf,
	IN  Vitdec_Mode mode,
	IN  Uint32 num_bits_to_decode
	){
	conf->mode = mode;
	conf->num_bits_to_decode = num_bits_to_decode;
}
