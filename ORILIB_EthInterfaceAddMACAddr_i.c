#include <osl/inc/swpform.h>
#include <osl/inc/eth/eth.h>
#include "ORILIB_util.h"
#include "ORILIB_EthInterfaceAddMACAddr_t.h"
#include <osl/eth/cpsw_singlecore.h>

void ORILIB_EthInterfaceAddMACAddr_i (
		CF ORILIB_t_EthInterfaceAddMACAddr * conf
	) {
	if (Setup_PASS (conf->mac) != 0)
	{
		printf ("PASS setup failed \n");
		//return -1;
	}
	else
	{
		printf ("PASS is listening on mac addr "); 
		printMacAddr(conf->mac);
		printf("\n");
	}

	//return 0;
}


void ORILIB_EthInterfaceAddMACAddr_i_conf (
		CF ORILIB_t_EthInterfaceAddMACAddr * conf,
		Uint8 * mac
	) {
	memcpy(conf->mac, mac, 6);
}
