/**
Atomix project, ORILIB_PLCPParser_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

//IN BitsPacked_ll const * const restrict plcp_bits_in,
//INOUT WifiPlcpParserState * const restrict outPlcpState
//){

#include "ORILIB_PLCPParser_t.h"


void ORILIB_PLCPParser_i (
	    IN  ORILIB_t_ByteBuf4 const * const restrict inpBufPLCP,
	    OUT ORILIB_t_PLCPParserState * const restrict outPlcpState,
	    OUT WIFILIB_TxRxPktState * const restrict outPktState,
	    OUT Decision_t *bOutDecision,
	    CF  ORILIB_t_PLCPParserConf *conf
	    ){

	    //OUT Vitdec_ViterbiDecodingMap * const restrict vitdecMap

	//plcp symbol hard-bits after viterbi decoding
	DEBUG_DATA(
	LOG_PRINTF("plcp decoded bits\n");
	Uint32 index;
	Uint32 *hard_decision = (Uint32 *)inpBufPLCP->bytes;
	for (index = 0; index < 1; index++) {
		printBitsMsb(hard_decision[index]);
		LOG_PRINTF("%d\n", hard_decision[index]);
	}
	)



	//read the upper word of the input, since there's only 24 bits, this is fine.
	//uint32_t bits_of_interest = _amem4_const(plcp_bits_in);
	uint32_t bits_of_interest = _amem4_const(inpBufPLCP->bytes);
	
	_amem4(&(outPlcpState->data_rate_4bits)) = _extu(bits_of_interest,0,28);
	
	_amem4(&(outPlcpState->packet_length_12bits)) = (_bitr(_extu(bits_of_interest,5,20))>>20);


	ORILIB_t_PLCPParserState * plcp_result = outPlcpState;
	WifiGeeDataRates data_rate = getWifiGeeDataRate(plcp_result->data_rate_4bits);
	Uint32 data_length = plcp_result->packet_length_12bits;		
		//number of data bytes (PHY payload, so this includes 
		//MAC layer's CRC32 because that is part of PHY payload)
	
	//!!!!!!!!!!!!!!!!!!!!!!
	//HACK: Hard-coding length for profiling.
	//data_length = 1512;
	////data_length = 120;
	//data_rate = kWifiGee54mbps;
	//!!!!!!!!!!!!!!!!!!!!!!

	//!!!!!!!!!!!!!!!!!!!!!!
	//HACK: Hard-coding length to bad value for forcing continue.
	//data_length = 1600;
	//!!!!!!!!!!!!!!!!!!!!!!

#ifdef PLCPSTATICPKTLEN
#if PLCPSTATICPKTLEN > 0
	data_length = PLCPSTATICPKTLEN;
#endif
#endif


	DEBUG_INFO(
	LOG_PRINTF("datarate_plcp: %2d, datarate_idx : %2d, data length (bytes) : %4d\n", 
	  plcp_result->data_rate_4bits,
	  data_rate,
	  data_length);
	)


	WIFILIB_setPktState(
		outPktState, 
		data_rate, 
		data_length
		);

	
	outPlcpState->nOfdmSymsInPkt = outPktState->NumDataSymbols;

	//--------------------------
	Uint32 nextState;
	Uint32 iterCount;

	Uint32 *transitionMap;
	transitionMap = conf->transitionMap;

	switch(data_rate) {
	case kWifiGee6mbps:
		//DEBUG_INFO(
		//LOG_PRINTF("PLCPOKK: decodable data rate  6Mbps");
		//)
		//nextState = transitionMap[1];

		//TODO: Turning off 6mbps decode path too because we have not tested it yet
		DEBUG_INFO(
		LOG_PRINTF("PLCPERR: unhandled data rate  6Mbps");
		)
		nextState = transitionMap[0];

		iterCount = 1;
		break;
	case kWifiGee9mbps:
		DEBUG_INFO(
		LOG_PRINTF("PLCPERR: unhandled data rate  9Mbps");
		)
		nextState = transitionMap[0];
		iterCount = 1;
		break;
	case kWifiGee12mbps:
		DEBUG_INFO(
		LOG_PRINTF("PLCPERR: unhandled data rate 12Mbps");
		)
		nextState = transitionMap[0];
		iterCount = 1;
		break;
	case kWifiGee18mbps:
		DEBUG_INFO(
		LOG_PRINTF("PLCPERR: unhandled data rate 18Mbps");
		)
		nextState = transitionMap[0];
		iterCount = 1;
		break;
	case kWifiGee24mbps:
		DEBUG_INFO(
		LOG_PRINTF("PLCPERR: unhandled data rate 24Mbps");
		)
		nextState = transitionMap[0];
		iterCount = 1;
		break;
	case kWifiGee36mbps:
		DEBUG_INFO(
		LOG_PRINTF("PLCPERR: unhandled data rate 36Mbps");
		)
		nextState = transitionMap[0];
		iterCount = 1;
		break;
	case kWifiGee48mbps:
		DEBUG_INFO(
		LOG_PRINTF("PLCPERR: unhandled data rate 48Mbps");
		)
		nextState = transitionMap[0];
		iterCount = 1;
		break;
	case kWifiGee54mbps:
		DEBUG_INFO(
		LOG_PRINTF("PLCPOKK: decodable data rate 54Mbps");
		)
		if (data_length <= 24) {
		  nextState = transitionMap[3];	//single ofdm symbol
		}
		else {
		  nextState = transitionMap[2];
		}
		iterCount = 1;
		break;
	default:
		DEBUG_INFO(
		LOG_PRINTF("PLCPERR: undefined data rate ??Mbps");
		)
		//exit(1);
		nextState = transitionMap[0];
		iterCount = 1;
	}

	DEBUG_INFO(
	LOG_PRINTF(", ");
	)
	if (data_length > 1514) {
		DEBUG_INFO(
		  LOG_PRINTF("PLCPERR: crap data length %4d, skipping...\n", data_length);
		)
		nextState = transitionMap[0];
		iterCount = 1;
	}
	else {
		DEBUG_INFO(
		if (nextState == transitionMap[0]) {
		  LOG_PRINTF("PLCPOKK: good data length %4d, skipping...\n", data_length);
		}
		else {
		  LOG_PRINTF("PLCPOKK: good data length %4d, decoding...\n", data_length);
		}
		)
	}

	//TODO: turning off decode of pkts with less than 6 ofdm symbols because we have not tested onesymrx path yet
	DEBUG_INFO(
	LOG_PRINTF(", ");
	)
	if (outPlcpState->nOfdmSymsInPkt < 6) {
		DEBUG_INFO(
		  LOG_PRINTF("PLCPERR: less than 6 ofdm symbols not handled, skipping...\n");
		)
		nextState = transitionMap[0];
		iterCount = 1;
	}

	bOutDecision->nextState = nextState;
	bOutDecision->iterCount = iterCount;

	DEBUG_VERBOSE(
	    LOG_PRINTF("ORILIB_PLCPParser: nextState=%d, iterCount=%d\n", 
	      nextState, iterCount);
	)

}

void ORILIB_PLCPParser_i_conf (
	CF  ORILIB_t_PLCPParserConf *conf,
	Uint32 stContinue,
	Uint32 st6m,
	Uint32 st54m,
	Uint32 st54m_onesymrx
	) {
  Uint32 *transitionMap;
  transitionMap = conf->transitionMap;

  transitionMap[0] = stContinue;
  transitionMap[1] = st6m;
  transitionMap[2] = st54m;
  transitionMap[3] = st54m_onesymrx;
}
