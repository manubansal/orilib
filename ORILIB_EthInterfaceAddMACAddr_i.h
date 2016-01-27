#ifndef ORILIB_ETHINTERFACEADDMACADDR_I_H
#define ORILIB_ETHINTERFACEADDMACADDR_I_H 

#include <osl/inc/swpform.h>
#include <osl/inc/eth/eth.h>
#include "ORILIB_util.h"
#include "ORILIB_EthInterfaceAddMACAddr_t.h"

void ORILIB_EthInterfaceAddMACAddr_i (
		CF ORILIB_t_EthInterfaceAddMACAddr * conf
		);

void ORILIB_EthInterfaceAddMACAddr_i_conf (
		CF ORILIB_t_EthInterfaceAddMACAddr * conf,
		Uint8 * mac
		);

#endif /* ORILIB_ETHINTERFACEADDMACADDR_I_H */
