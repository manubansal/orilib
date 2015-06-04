/**
Atomix project, ORILIB_PeakDetector_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#if SYNC_BUFFER_SIZE_ENERGY != 80
#error: Need SYNC_BUFFER_SIZE_ENERGY to be 80
#endif



static void inner(
		//-- energy-only related fields --
		//IN 		Cplx16 const freshSampleBuf_noGain[restrict],			
		//new buffer of samples, length SYNC_BUFFER_SIZE_ENERGY
		IN 		Cplx16 const freshSampleBuf_withGain[restrict],			
		//new buffer of samples, length SYNC_BUFFER_SIZE_ENERGY


		//-- peak-detection specific fields --
		INOUT	Uint32	*maxMetricRunLength,
		INOUT	Uint32	*maxMetricIsHigh,
		INOUT	Uint32	*maxMetricBufIdx,
		INOUT	Uint32	*maxMetric,


		INOUT	Uint32	*peakFound,


		//-- for debugging --
		//IN		float	debug_metricFBuf[restrict],

		//-- not just for debugging --
		IN		Uint32	debug_metricBuf[restrict],

		//-- not just for debugging --
		IN		Uint32	debug_magNumerBuf[restrict],

		//-- not just for debugging --
		IN		Uint32	debug_magDenomBuf[restrict],

		//-- for debugging --
		//OUT		Uint32	debug_isMetricHighBuf[restrict]

		INOUT	Cplx16	alignedSampleLookbackBuf[restrict]

		){

    Uint32	lo_maxMetricRunLength 	= *maxMetricRunLength;
    Uint32	lo_maxMetricIsHigh 	= *maxMetricIsHigh;
    Uint32	lo_maxMetricBufIdx 	= *maxMetricBufIdx;
    Uint32	lo_maxMetric 		= *maxMetric;

    Uint32	lo_peakFound 		= *peakFound;

    Uint32 i;

#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY,SYNC_BUFFER_SIZE_ENERGY, SYNC_BUFFER_SIZE_ENERGY);
    for ( i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i++ ) {
	Cplx16U lo_currSample_withGain;
	//Cplx16U lo_currSample_noGain;
	Uint32 lo_magASqSqTmp, lo_magESqSqTmp, lo_magESqSqTmpScaled;
	Uint32 lo_currMetric;
	Uint32 lo_isMetricHigh;

	lo_currSample_withGain.realimag 	= _amem4(&freshSampleBuf_withGain[i]);		
	//Q15 (16bit signed samples in (-1.0,1.0)
	//lo_currSample_noGain.realimag	= _amem4_const(&freshSampleBuf_noGain[i]);
	lo_magASqSqTmp 			= _amem4(&debug_magNumerBuf[i]);
	lo_magESqSqTmp 			= _amem4(&debug_magDenomBuf[i]);
	lo_currMetric 			= _amem4(&debug_metricBuf[i]);


	//---------------------- begin metric-threshold comparison ----------------------

	//TODO: Our metric values are significantly lower than corresponding
	//matlab testbench because of which we have a lower cutoff threshold
	//too. The reason for these lower values need to be explore further.
	//The only difference seems to be finite precision computation/fixed
	//point arithmetic. If we don't use the lower threshold value, we miss
	//packets.

	//magESqSqTmpScaled = magESqSqTmp - (magESqSqTmp >> 2); 
	//doing 3/4th, which is our first threshold
	lo_magESqSqTmpScaled = lo_magESqSqTmp >> 2; 
	//doing 1/4, which is our first threshold
	//magESqSqTmpScaled = magESqSqTmp >> 1; 
	//doing 1/2, which is our first threshold
	//lo_magESqSqTmpScaled = lo_magESqSqTmp >> 1; 
	//doing 1/2, which is our first threshold

	//magESqSqTmpScaled = (magESqSqTmp >> 1) - (magESqSqTmp >> 3); 
	//doing 3/8, which is our first threshold. magic!

	lo_isMetricHigh = lo_magESqSqTmpScaled && (lo_magASqSqTmp >= lo_magESqSqTmpScaled);
	//DEBUG(LOG_PRINTF("ratio: %ld\n", ratio >> 21 );) 
	//to normalize to 1 = 1024, making easier to read off values
	//DEBUG(LOG_PRINTF("sno %5d		%15lu		%15lu		%15lu\n", syncState->seenCounter, magASqSqTmp, magESqSqTmp, ratio);)

	//---------------------- done metric-threshold comparison ----------------------

	//---------------------- begin peak-detection computation ----------------------

	//irrespective of whether the metric is over the threshold or not,
	//we can maintain the maximum metric seen so far (the candidate for
	//a peak) and its dominance run length

	//if lo_currMetric >= lo_maxMetric, then we are resetting the peak
	//candidate point to the current point, otherwise we are continuing
	//the previous max-metric point. it could either be above the
	//threshold (true candidate) or below (false threshold). only in
	//the case that it is a true candidate do we want to write to the
	//aligned sample buffer using it's run length.

	lo_maxMetricRunLength = lo_currMetric >= lo_maxMetric ? 0 : (lo_maxMetricRunLength + 1);
	//reset or increment
	//if we are resetting to the current point, also reset the run
	//length, otherwise we are retaining the previous candidate point,
	//so the run length increments.

	lo_maxMetricIsHigh 
		= lo_currMetric >= lo_maxMetric ? lo_isMetricHigh : lo_maxMetricIsHigh;	
	//accordingly pick the threshold comparison value
	//if we are resetting the candidate, we want to use the current
	//isMetricHigh status, otherwise we retain the status, which was
	//set whenever the max metric point was last set.

	lo_maxMetricBufIdx = lo_currMetric >= lo_maxMetric ? i : lo_maxMetricBufIdx;
	//set to current or retain
	//similar to above - reset or retain.

	lo_maxMetric = lo_currMetric >= lo_maxMetric ? lo_currMetric : lo_maxMetric;
	//set to current or retain
	//the actual maxMetric value, similarly reset or retained.

	//If maxMetricIsHigh, it is a peak candidate, so a potential start of frame.
	//Continue to accumulate samples from the peak in anticipation of timing lock.
	//In order to keep the runtime deterministic, we choose the index accordingly,
	//not whether or not to write to memory.
//#if (SYNC_ALIGNED_SAMPLES_WITH_GAIN == 1)
	_amem4(&alignedSampleLookbackBuf[lo_maxMetricIsHigh ? lo_maxMetricRunLength : 0])	
		= lo_currSample_withGain.realimag;
//#else
//	_amem4(&alignedSampleLookbackBuf[lo_maxMetricIsHigh ? lo_maxMetricRunLength : 0])	
//		= lo_currSample_noGain.realimag;
//#endif

	//now interpret the running metric properties to conclude peak detection
	lo_peakFound 
		= (lo_maxMetricIsHigh && (lo_maxMetricRunLength >= SYNC_PEAK_DOMINANCE_LENGTH));
	//is current scenario implying a peak?
	/* Note: The above peak-conclusion mechanism favors the dominant
	 * value across the whole explored buffer.  If a peak has been
	 * found and a yet-higher metric is seen in the buffer later on, it
	 * will oust the earlier peak, even though the next max-point may
	 * not even have enough run length in the buffer. In that case,
	 * this buffer will return declaring peak not found while a peak
	 * did actually exist in the buffer. However, this is an extremely
	 * unlikely case if the buffer size is comparable to runlength
	 * threshold/peak search window size, which will almost always be
	 * the case.
	 */


	//---------------------- done peak-detection computation ----------------------

	//for debugging
//#ifdef DEBUG_MODE
//	_amem4(&debug_isMetricHighBuf[i]) = lo_isMetricHigh;
//#endif

	//increment sample pointer marker
	//yPtr++;
    }

//#define CONSTSHIFT -1
#define CONSTSHIFT 0
    //for peak-detection
    *maxMetricRunLength = lo_maxMetricRunLength - CONSTSHIFT;
    *maxMetricIsHigh 	= lo_maxMetricIsHigh;
    *maxMetricBufIdx 	= lo_maxMetricBufIdx - CONSTSHIFT;
    *maxMetric 		= lo_maxMetric;

    *peakFound 		= lo_peakFound;

}

void ORILIB_PeakDetector_i (
		IN  	ORILIB_t_Cplx16Buf80 const 	* const restrict sampleBufWithGain,
		IN	ORILIB_t_GainStateAux * gainStateAux,
		IN 	ORILIB_t_AlignState * alignState,
		IN 	ORILIB_t_CorrState * corrState,
		OUT 	ORILIB_t_DetectState * outDetectState
		){

    ComplexSamples const * raw_samples_with_gain = sampleBufWithGain->samples;

    inner(
	//-- energy-only related fields --
	//IN 	raw_samples,			
		//new buffer of samples, length SYNC_BUFFER_SIZE_ENERGY

	IN 	raw_samples_with_gain,			
		//new buffer of samples, length SYNC_BUFFER_SIZE_ENERGY


	//-- peak-detection specific fields --
	INOUT	&corrState->corr_maxMetricRunLength,
	INOUT	&corrState->corr_maxMetricIsHigh,
	INOUT	&corrState->corr_maxMetricBufIdx,
	INOUT	&corrState->corr_maxMetric,


	INOUT	&corrState->corr_peakFound,


	//-- for debugging --
	//IN	corrState->debug_metricFBuf,

	//-- not just for debugging --
	IN	corrState->debug_metricBuf,

	//-- not just for debugging --
	IN	corrState->debug_magNumerBuf,

	//-- not just for debugging --
	IN	corrState->debug_magDenomBuf,

	//-- for debugging --
	//OUT	corrState->debug_isMetricHighBuf


	//OUT	&alignState->alignedSampleLookbackBuf[SYNC_BUFFER_SIZE_ENERGY - 1],	
	//MB: -1 is to allow discarding the last symbol of stf 
	//that corr block writes in. this is for our creative 
	//alignment scheme to work.
	//MB 12/06/2013: I don't remember at all what this creative
	//scheme is. I am reverting this change because it is causing
	//the first aligned sample buffer to have part of the buffer
	//be one sample ahead of rest of the buffer, effectively inserting
	//one sample in the middle of an otherwise perfect aligned sample
	//buffer.
	OUT	&alignState->alignedSampleLookbackBuf[SYNC_BUFFER_SIZE_ENERGY]

	);

    //alignState->nAlignedSamplesAlreadyFilled = 
    //	corrState->corr_peakFound ? corrState->corr_maxMetricRunLength : 0;

    //The following adjustment is a HACK. I'm not sure why this should be needed, though it is not outrageous.
    //It seems to be an issue of indexing in peak correlation detection algorithm. --MB, 01/22/2014
    alignState->nAlignedSamplesAlreadyFilled = 
	    corrState->corr_peakFound ? corrState->corr_maxMetricRunLength + 1 : 0;

    alignState->nAlignedSamplesAvailable = alignState->nAlignedSamplesAlreadyFilled;


    outDetectState->peakFound = corrState->corr_peakFound;
    outDetectState->agcGain   = gainStateAux->currAgcGain;

    DEBUG_INFO(
    if (outDetectState->peakFound) {
      LOG_PRINTF("^^^^^^^^^^^^^^ Correlation peak found ^^^^^^^^^^^^^^^^^^\n");
      LOG_PRINTF("############## agcGain: %3d ##############\n", outDetectState->agcGain);
    }
    )
}
