/**
Atomix project, ORILIB_ChannelImpulseResponse_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/



#include "ORILIB_ChannelImpulseResponse_t.h"
#include <ti/dsplib/src/DSPF_sp_mat_mul_cplx/DSPF_sp_mat_mul_cplx.h>

//#pragma DATA_SECTION(LTF_DECONVOLUTION_MATRIX, ".ddr");
#pragma DATA_ALIGN(y, 8);
#pragma DATA_ALIGN(h, 8);
static float y[REFERENCE_VECTOR_LENGTH * 2];
static float h[CHANNEL_TAP_LENGTH * 2];
//static float hhstar[CHANNEL_TAP_LENGTH];
static float hhstar[ACCURATE_CHANNEL_RESPONSE_LENGTH];
static float hhstar_normalized[ACCURATE_CHANNEL_RESPONSE_LENGTH];

void my_matrix_multiply_with_vector(float* x1, const int r1, const int c1, const float* x2,
	const int c2, float* restrict y) {
	Uint32 i, j;
	if (c2 != 1) {
	  printf("ERROR: need a vector (c2 == 1) \n");
	  exit(1);
	}
	for (i = 0; i < r1 * 2; i+= 2) {
	  float pI = 0, pQ = 0;
	  for (j = 0; j < c1 * 2; j+= 2) {
	    float aI = x1[i * c1 + j];
	    float aQ = x1[i * c1 + j + 1];
	    float bI = x2[j];
	    float bQ = x2[j + 1];
	    pI += aI * bI - aQ * bQ;
	    pQ += aI * bQ + aQ * bI;
	  }
	  y[i] = pI;
	  y[i+1] = pQ;
	}
}

void ORILIB_ChannelImpulseResponse_i(
	IN  ORILIB_t_Cplx16Buf160 * restrict inpLtfsBuf
	){

  Uint32 i, j;
  j = 0;
  for (i = 0; i < REFERENCE_VECTOR_LENGTH * 2; i+=2) {
    y[i] = inpLtfsBuf->samples[
    	j + (CHANNEL_TAP_LENGTH - 1 + DISTORTION_AVOIDANCE_OFFSET - 1)].real;
    y[i+1] = inpLtfsBuf->samples[
    	j + (CHANNEL_TAP_LENGTH - 1 + DISTORTION_AVOIDANCE_OFFSET - 1)].imag;
    j += 1;
  }

  //void DSPF_sp_mat_mul_cplx(float* x1, const int r1, const int c1, const float* x2,
  //	const int c2, float* restrict y)
  //DSPF_sp_mat_mul_cplx(	--stupid function, doesn't work!
  my_matrix_multiply_with_vector(
  	LTF_DECONVOLUTION_MATRIX,
	CHANNEL_TAP_LENGTH,
	REFERENCE_VECTOR_LENGTH,
	y,
	1,
	h
	);


  j = 0;
  //for (i = 0; i < CHANNEL_TAP_LENGTH * 2; i+=2) {
  float E = 0;
  for (i = 0; i < ACCURATE_CHANNEL_RESPONSE_LENGTH * 2; i+=2) {
    float I = h[i];
    float Q = h[i+1];
    float e = I*I + Q*Q;
    E += e;
    hhstar[j] = e;
    j++;
  }

  j = 0;
  for (i = 0; i < ACCURATE_CHANNEL_RESPONSE_LENGTH * 2; i+=2) {
    hhstar_normalized[j] = hhstar[j]/E;
    //1e4 corresponds to 40dB dynamic range 
    printf("j=%3d, e=%10.2f\n", j, 10000 * hhstar_normalized[j]); 	
    j++;
  }
}
