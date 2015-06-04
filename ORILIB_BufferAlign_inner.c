/**
Atomix project, ORILIB_BufferAlign_inner.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/amem4cpy_inl.h>

void ORILIB_BufferAlign_inner(
	Cplx16 const * const restrict unaligned_raw_samples, 
	ORILIB_t_AlignState * alignStateInpOut,
	Cplx16 * const restrict aligned_raw_samples
	){

#ifdef DEBUG_MODE
	assert(alignStateInpOut->nAlignedSamplesAlreadyFilled <= SYNC_BUFFER_SIZE_ENERGY * 2);
	//implementing the same scheme without this condition will take more thought, which i
	//didn't want to put in at date(then).

	assert(SYNC_ALIGNED_SAMPLE_BUF_LEN_ACTUAL >= 4 * SYNC_BUFFER_SIZE_ENERGY);
#endif


	//get indices
	Uint32 uaks1 = _amem4(&alignStateInpOut->uaks1);
	Uint32 uaks2 = _amem4(&alignStateInpOut->uaks2);
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
	//write1
	Cplx16 * const restrict dst1 = &alignedSampleBufOut[uai1];
#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY, SYNC_BUFFER_SIZE_ENERGY, SYNC_BUFFER_SIZE_ENERGY);
	for (i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i++) _amem4(&dst1[i]) = _amem4_const(&unaligned_raw_samples[i]);
}


	//read
{
	Cplx16 * const restrict src = &alignedSampleBufOut[uaks1];
#ifdef DEBUG_MODE
	ASSERT_PTR_ALIGNED(src, 8);
	ASSERT_PTR_ALIGNED(aligned_raw_samples, 8);
#endif
#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY/2, SYNC_BUFFER_SIZE_ENERGY/2, SYNC_BUFFER_SIZE_ENERGY/2);
	for (i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i+=2) _amem8(&aligned_raw_samples[i]) = _amem8(&src[i]);
}

{	
	//mv1
	Cplx16 * const restrict src = &alignedSampleBufOut[uaks2];
	Cplx16 * const restrict dst = &alignedSampleBufOut[uaks1];
#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY/2, SYNC_BUFFER_SIZE_ENERGY/2, SYNC_BUFFER_SIZE_ENERGY/2);
	for (i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i+=2) _amem8(&dst[i]) = _amem8(&src[i]);
}

{	
	//mv2
	Cplx16 * const restrict src = &alignedSampleBufOut[uaks2 + SYNC_BUFFER_SIZE_ENERGY];
	Cplx16 * const restrict dst = &alignedSampleBufOut[uaks1 + SYNC_BUFFER_SIZE_ENERGY];
#pragma MUST_ITERATE(SYNC_BUFFER_SIZE_ENERGY/2, SYNC_BUFFER_SIZE_ENERGY/2, SYNC_BUFFER_SIZE_ENERGY/2);
	for (i = 0; i < SYNC_BUFFER_SIZE_ENERGY; i+=2) _amem8(&dst[i]) = _amem8(&src[i]);
}

	alignStateInpOut->nAlignedSamplesAvailable = 
		alignStateInpOut->nAlignedSamplesAvailable + SYNC_BUFFER_SIZE_ENERGY;
}
