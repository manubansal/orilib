/**
Atomix project, ORILIB_printDebug.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

/*
 * test_printDebug.c
 *
 *  Created on: Feb 8, 2012
 *      Author: Manu Bansal
 */

#include <stdio.h>
#include "WIFILIB_parameters.h"
#include <osl/inc/util.h>

void TEST_printDebug_txRxPktState(WIFILIB_TxRxPktState *txRxPktState) {
	LOG_PRINTF("txRxPktState->NumInfoBits                    : %d\n",	     txRxPktState->NumInfoBits);
	LOG_PRINTF("txRxPktState->DataRate                       : %d\n",        txRxPktState->DataRate);
	LOG_PRINTF("txRxPktState->ModulationSchemeIndex          : %d\n",        txRxPktState->ModulationSchemeIndex);
	LOG_PRINTF("txRxPktState->NumCodedBitsPerCarrier         : %d\n",        txRxPktState->NumCodedBitsPerCarrier);
	LOG_PRINTF("txRxPktState->CodingRateTimes120             : %d\n",        txRxPktState->CodingRateTimes120);
	LOG_PRINTF("txRxPktState->NumCodedBitsPerSymbol          : %d\n",        txRxPktState->NumCodedBitsPerSymbol);
	LOG_PRINTF("txRxPktState->NumDataBitsPerSymbol           : %d\n",        txRxPktState->NumDataBitsPerSymbol);
	LOG_PRINTF("txRxPktState->NumUncodedBits                 : %d\n",        txRxPktState->NumUncodedBits);
	LOG_PRINTF("txRxPktState->NumCodedBits                   : %d\n",        txRxPktState->NumCodedBits);
	LOG_PRINTF("txRxPktState->NumPadBits                     : %d\n",        txRxPktState->NumPadBits);
	LOG_PRINTF("txRxPktState->NumConstPnts                   : %d\n",        txRxPktState->NumConstPnts);
	LOG_PRINTF("txRxPktState->NumDataSymbols                 : %d\n",        txRxPktState->NumDataSymbols);
}

void TEST_printDebug_descrambledBits(Uint8 *bytes, Uint32 nBytes) {
	int i;
	for (i = 0; i < nBytes; i++) {
		if (i % 4 == 0 && i > 0)	LOG_PRINTF("\n");
		printBitsMsb8(bytes[i]); LOG_PRINTF(" ");
	}
	LOG_PRINTF("\n");
}

void TEST_printDebug_decodedBits(Uint8 *bytes, Uint32 nBytes) {
	int i;
	for (i = 0; i < nBytes; i++) {
		if (i % 4 == 0 && i > 0)	LOG_PRINTF("\n");
		printBitsMsb8(bytes[i]); LOG_PRINTF(" ");
	}
	LOG_PRINTF("\n");
}
