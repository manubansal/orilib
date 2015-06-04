/**
Atomix project, ORILIB_BPSKEVMEstimator_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_BPSKEVMEstimator_t.h"

#define PLUS_ONE	96

void ORILIB_BPSKEVMEstimator_i (
    IN ORILIB_t_Cplx16Buf48	* bufOutEqualizedPnts
    ){
    double evm = 0;
    Uint32 i;

    double e = 0;				//sum of error vectors energies
    for (i = 0; i < 48; i++) {
      Int32 I = bufOutEqualizedPnts->samples[i].real;
      Int32 Q = bufOutEqualizedPnts->samples[i].imag;
      double eI = abs(PLUS_ONE - abs(I));
      double eQ = abs(Q);
      e += eI * eI + eQ * eQ;
    }
    double s = PLUS_ONE * PLUS_ONE * 48;	//sum of signal vector energies
    double evm_rms = sqrt(e/s);
    double snr_lin_from_evm = s/e;
    double snr_dB_from_evm = -20 * log10(evm_rms);
    printf("PLCP: evm_rms = %f, snr_lin_from_evm = %f, snr_dB_from_evm = %f\n", 
    	evm_rms, snr_lin_from_evm, snr_dB_from_evm);
}
