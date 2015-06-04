/**
Atomix project, ORILIB_ByteCachePut_a8_C120_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_BYTECACHEPUT_A8_C120_I_H 


void ORILIB_ByteCachePut_a8_C120_i (
	IN  void			* inpBuf,
	IN  ORILIB_t_ByteCache120	* inpCache,
	OUT ORILIB_t_ByteCache120	* outCache,
	CF  ORILIB_t_ByteCache120_Conf	* conf
	);

void ORILIB_ByteCachePut_a8_C120_i_conf (
	CF  ORILIB_t_ByteCache120_Conf	* conf,
	IN  Uint32 nBytes
	);

#endif /* ORILIB_BYTECACHEPUT_A8_C120_I_H */
