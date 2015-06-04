/**
Atomix project, ORILIB_PreambleDetector_Init_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

void ORILIB_PreambleDetector_Init_i(
	OUT ORILIB_t_CorrState   * corrState,
	OUT ORILIB_t_PktAgcState * gainState 
	){

	Uint32 i;

	gainState->agcGain   = 0;

	//zero-initialize buffers
	corrState->corr_zwinPosition = 0;

	//for (i = 0; i < SYNC_BUFFER_SIZE; i++) {
	//	corrState->debug_isMetricHighBuf[i] = 0;
	//}

	for (i = 0; i < SYNC_WINDOW_SIZE_ENERGY; i++) {
		corrState->corr_zwinSampleBuf[i].real = 0;
		corrState->corr_zwinSampleBuf[i].imag = 0;
		corrState->corr_zwinCorrTermBuf[i].real = 0;
		corrState->corr_zwinCorrTermBuf[i].imag = 0;
	}

	corrState->corr_currWinCorr.real = 0;
	corrState->corr_currWinCorr.imag = 0;

	corrState->corr_maxMetricRunLength = 0;
	corrState->corr_maxMetricIsHigh = 0;
	corrState->corr_maxMetricBufIdx = 0;
	corrState->corr_maxMetric = 0;

	corrState->corr_peakFound = 0;
}
