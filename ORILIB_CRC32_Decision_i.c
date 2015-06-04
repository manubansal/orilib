/**
Atomix project, ORILIB_CRC32_Decision_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include "ORILIB_CRC32_t.h"
#ifndef DEVICE_K2K
#include <ti/platform/evmc6670l/platform_lib/include/evmc66x_fpga.h>
#endif

far Uint32 ORILIB_CRC32_Decision_nPass = 0;
far Uint32 ORILIB_CRC32_Decision_nFail = 0;

void ORILIB_CRC32_Decision_i (
	IN  ORILIB_t_CRC32_State 	* inpState,
	OUT Decision_t 			* bOutDecision,
	CF  ORILIB_t_CRC32_DecisionConf * bInpConf
	){

  Uint32 crcValue = inpState->crcValue;
  Uint32 match = crcValue == 0x38FB2284;

  Uint32 nextState, iterCount;

  if (match) {
#ifndef SNRBER
   DEBUG_INFO(
#endif
   printf("!!!!!!!!!!!!!! CRC CORRECT !!!!!!!!!!!!!!!!!\n");
    //static Uint8 a = 0, a_led = 0;
    //a_led = (a++ >> 5) & 0x1;
//#ifndef DEVICE_K2K
    //if (a_led)
    //	fpgaControlUserLEDs(FPGA_USER_LED0, FPGA_LED_ON);
    //else
    //	fpgaControlUserLEDs(FPGA_USER_LED0, FPGA_LED_OFF);
//#endif
#ifndef SNRBER
    )
#endif
    //TODO: this is a hack to suppress ack tx if crc matches
    nextState = bInpConf->transitionMap[0];
    //nextState = bInpConf->transitionMap[1];
    iterCount = 1;

    ORILIB_CRC32_Decision_nPass++;
  }
  else {
    DEBUG_ERROR(
    printf("XXXXXXXXXXXXXX CRC FAILED  XXXXXXXXXXXXXXXXX\n");
		static Uint8 b = 0;
		//fpgaControlUserLEDs(FPGA_USER_LED1, ((b++ >> 5) & 0x1 ? FPGA_LED_OFF : FPGA_LED_ON));
    )
    nextState = bInpConf->transitionMap[1];
    iterCount = 1;

    ORILIB_CRC32_Decision_nFail++;
  }

  bOutDecision->nextState = nextState;
  bOutDecision->iterCount = iterCount;
}

void ORILIB_CRC32_Decision_i_conf (
	CF  ORILIB_t_CRC32_DecisionConf * bInpConf,
	IN  Uint32 stTxAck,
	IN  Uint32 stRxFinishWithoutTxAck
	) {
  Uint32 *transitionMap;
  transitionMap = bInpConf->transitionMap;

  transitionMap[0] = stTxAck;
  transitionMap[1] = stRxFinishWithoutTxAck;
}
