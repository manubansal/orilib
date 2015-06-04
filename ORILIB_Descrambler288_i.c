/**
Atomix project, ORILIB_Descrambler288_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

void ORILIB_Descrambler288_i (
	IN  ORILIB_t_BitBucket288 		* const restrict inpBuf, 
	IN  ORILIB_t_Descrambler_State 		* restrict inpStateBuf, 
	OUT ORILIB_t_BitBucket288 		* const restrict outBuf, 
	OUT ORILIB_t_Descrambler_State 		* restrict outStateBuf
	){

	//uint32_t data_length_in_bits = 288;
	// Determine how many 64 bit output frames are generated. This number is rounded up (from input bits/64)
	//uint32_t n_double_words = (data_length_in_bits>>6) + (data_length_in_bits & 0x3F ? 1:0); 
	//uint32_t n_double_words = kWifiGeeDataRatesInfo[state->data_rate].num_dword_containers_of_data_bits;
	//uint32_t n_double_words = 5;

	ORILIB_Descrambler_i_internal (
	      (uint64_t *)inpBuf->packedBits,
	      inpStateBuf,
	      (uint64_t *)outBuf->packedBits,
	      outStateBuf,
	      288,
	      5);


}


