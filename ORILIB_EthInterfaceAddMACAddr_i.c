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

	int i;
	int values[6];
	uint8_t bytes[6];

	//if( 6 == sscanf(mac, "%x:%x:%x:%x:%x:%x%c",
	if( 6 == sscanf(mac, "%x.%x.%x.%x.%x.%x%c",
	    &values[0], &values[1], &values[2],
	    &values[3], &values[4], &values[5] ) )
	{
	    /* convert to uint8_t */
	    for( i = 0; i < 6; ++i )
		bytes[i] = (uint8_t) values[i];

	    memcpy(conf->mac, bytes, 6);
	}

	else
	{
	    /* invalid mac */
	    printf("PASS is not registering additional mac -- invalid mac\n");
	}

}
