/**
Atomix project, ORILIB_beta_BufferAlign_inner.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "ORILIB_t.h"
#include <osl/inc/amem4cpy_inl.h>

	//ORILIB_t_beta_AlignState * alignStateInpOut,
void ORILIB_beta_BufferAlign_inner(
	Cplx16 const * const restrict unaligned_raw_samples, 
	ORILIB_t_AlignState * alignStateInpOut,
	Cplx16 * const restrict aligned_raw_samples,
	Uint32 nSamplesToSkip	//must be an even number
	){

#ifdef DEBUG_MODE
	assert(alignStateInpOut->nAlignedSamplesAlreadyFilled <= SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE * 2);
	//implementing the same scheme without this condition will take more thought, which i
	//didn't want to put in at date(then).

	assert(SYNC_ALIGNED_SAMPLE_BUF_LEN_ACTUAL >= 4 * SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE);
	assert(nSamplesToSkip % 2 == 0);
#endif


	//get indices
	Uint32 uaks1 = _amem4(&alignStateInpOut->uaks1);	//effective start point of good samples in the lookback window
	Uint32 uaks2 = _amem4(&alignStateInpOut->uaks2);	//effective start point of good samples to save (after appending 
								//the new input sample buffer to the lookback window)
	Uint32 offset = _amem4(&alignStateInpOut->nAlignedSamplesAlreadyFilled);

	Uint32 uai1 = uaks1 + offset;
	//Uint32 uai2 = uaks2 + offset;
	//uar = uak1, so need need to create a separate variable

	Cplx16 *alignedSampleBuf;

	Uint32 i;

	Cplx16 * alignedSampleBufOut = alignStateInpOut->alignedSampleLookbackBuf;


	//careful with copying pointers -- honor the restrict flags and do not copy
	//any pointers marked restriced.

{
	//write1: this block appends the entire new sample buffer into the lookback window 
	Cplx16 * const restrict dst1 = &alignedSampleBufOut[uai1];
#pragma MUST_ITERATE(SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE, SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE, SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE);
	for (i = 0; i < SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE; i++) _amem4(&dst1[i]) = _amem4_const(&unaligned_raw_samples[i]);
}


{
	//read: this block copies the output data from lookback window to the output buffer
	Cplx16 * const restrict src = &alignedSampleBufOut[uaks1 + nSamplesToSkip];	//TODO: need modulo?
#ifdef DEBUG_MODE
	ASSERT_PTR_ALIGNED(src, 8);
	ASSERT_PTR_ALIGNED(aligned_raw_samples, 8);
#endif
#pragma MUST_ITERATE(SYNC_ALIGNED_OUTPUT_SAMPLE_BUFFER_SIZE/2, SYNC_ALIGNED_OUTPUT_SAMPLE_BUFFER_SIZE/2, SYNC_ALIGNED_OUTPUT_SAMPLE_BUFFER_SIZE/2);
	for (i = 0; i < SYNC_ALIGNED_OUTPUT_SAMPLE_BUFFER_SIZE; i+=2) _amem8(&aligned_raw_samples[i]) = _amem8(&src[i]);
}

	uaks2 = uaks1 + nSamplesToSkip + SYNC_ALIGNED_OUTPUT_SAMPLE_BUFFER_SIZE;
{	
	//mv1: this block moves remaining samples in the lookback window to the start
	Cplx16 * const restrict src = &alignedSampleBufOut[uaks2];
	Cplx16 * const restrict dst = &alignedSampleBufOut[uaks1];
#pragma MUST_ITERATE(SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE/2, SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE/2, SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE/2);
	for (i = 0; i < SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE; i+=2) _amem8(&dst[i]) = _amem8(&src[i]);
}

{	
	//mv2: this block moves remaining samples in the lookback window to the start
	Cplx16 * const restrict src = &alignedSampleBufOut[uaks2 + SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE];
	Cplx16 * const restrict dst = &alignedSampleBufOut[uaks1 + SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE];
#pragma MUST_ITERATE(SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE/2, SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE/2, SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE/2);
	for (i = 0; i < SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE; i+=2) _amem8(&dst[i]) = _amem8(&src[i]);
}

	//not sure if this expression still holds
	alignStateInpOut->nAlignedSamplesAvailable = 
		alignStateInpOut->nAlignedSamplesAvailable + SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE;

	alignStateInpOut->nAlignedSamplesAlreadyFilled = 
		alignStateInpOut->nAlignedSamplesAlreadyFilled 
		+ SYNC_UNALIGNED_INPUT_SAMPLE_BUFFER_SIZE 
		- SYNC_ALIGNED_OUTPUT_SAMPLE_BUFFER_SIZE
		- nSamplesToSkip;
}
