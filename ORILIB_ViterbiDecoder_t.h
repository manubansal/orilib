/**
Atomix project, ORILIB_ViterbiDecoder_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef ORILIB_VITERTIDECODER_T_H_
#define ORILIB_VITERTIDECODER_T_H_

#include <osl/inc/or_types.h>
#include <osl/inc/swpform.h>

//#include <lib/vitdec/inc/vcpdriver/vcpdriver_api.h>
#include <osl/vitdec/inc/vcpdriver/vcpdriver_api.h>
#include "ORILIB_t.h"
#include "WIFILIB_t.h"

#define MAX_SYMBOL_LENGTH 216
#define MAX_INPUT_FRAME_SIZE (2*(MAX_SYMBOL_LENGTH + 2*CONVERGENCE_LENGTH))
#define MAX_OUTPUT_FRAME_SIZE 4 //In bits. TODO: FIX HACK, but this is 256 bits right now
#define CONVERGENCE_LENGTH 18 //TODO: This number might be higher or lower

typedef enum VcpInstance {
	kVCPI_0 = 0,
	kVCPI_1 = 1,
	kVCPI_2 = 2,
	kVCPI_3 = 3
} ORILIB_ViterDecoder_t_VcpInstance;

typedef struct {
	/* Driver state information */
	Vitdec_Profile 				profile 		__attribute__((aligned(8))); //output_frame_length derived from this
	VCP2_ConfigIc  				config  		__attribute__((aligned(8)));
	
	uint64_t 				output_params ;
	ORILIB_ViterDecoder_t_VcpInstance 	which_vcp  		__attribute__((aligned(8)));
	uint32_t 				cword  			__attribute__((aligned(8)));
	
	/** Actual output frame length. This is different for head, mid, and tail decoding. */
	uint32_t 				output_frame_length  	__attribute__((aligned(8)));
	
	/* For debug info */
	uint32_t 				num_bits_to_decode  	__attribute__((aligned(8)));
	uint32_t 				num_bits_decoded  	__attribute__((aligned(8)));
} ORILIB_t_ViterbiDecoderState;

//typedef struct {
//	ORILIB_ViterDecoder_t_VcpInstance 	which_vcp  		__attribute__((aligned(8)));
//} ORILIB_t_ViterbiDecoderIdx;


typedef struct {
	ORILIB_ViterDecoder_t_VcpInstance 	which_vcp;
	Uint32 					numSymProcExpected;
} ORILIB_t_ViterbiDecoderWFCState;

typedef struct {
	Vitdec_Mode mode;
	ORILIB_ViterDecoder_t_VcpInstance which_vcp;
	Vitdec_ConvergenceLength convergenceLength;
} ORILIB_t_ViterbiDecoderInitOnceConf;
	
typedef struct {
	ORILIB_ViterDecoder_t_VcpInstance which_vcp;
} ORILIB_t_ViterbiDecoderWFCConf;

//typedef struct {
//	Vitdec_Mode mode;
//	ORILIB_ViterDecoder_t_VcpInstance which_vcp;
//} ORILIB_t_ViterbiDecoderVCP_InitIdxConf;

typedef struct {
	Vitdec_Mode mode;
	Uint32 num_bits_to_decode;
	Vitdec_ConvergenceLength convergenceLength;
} ORILIB_t_ViterbiDecoderGenConfConf;


#endif


