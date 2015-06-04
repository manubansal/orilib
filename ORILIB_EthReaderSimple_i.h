/**
Atomix project, ORILIB_EthReaderSimple_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#define ORILIB_ETHREADERSIMPLE_I_H 

#include "ORILIB_EthReaderSimple_t.h"

void ORILIB_EthReaderSimple_i (
      OUT void * pkt,
      CF  ORILIB_EthReaderSimple_t_Conf * conf
      );

void ORILIB_EthReaderSimple_i_conf(
	CF  ORILIB_EthReaderSimple_t_Conf * conf,
	Uint32 nDblWords,
	Uint16 udpDstPort
	);

#endif /* ORILIB_ETHREADERSIMPLE_I_H */
