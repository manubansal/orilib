/**
Atomix project, ORILIB_SoftDepuncturerR3by4F216_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include "ORILIB_SoftDepuncturerR3by4F216_t.h"
#include "ORILIB_SoftDepuncturer_algos.h"

//F=216 is the number of data bits in the decoded message
//number of coded bits after depuncturing = (2/1) * F = 432
//number of coded bits before depuncturing = (4/3) * F = 288
//This function will take 288 input soft bits and give out 432 soft bits,
//which is a factor of (3/4)/(1/2) increase, that is, a 1.5x increase.
void ORILIB_SoftDepuncturerR3by4F216_i (
		IN  ORILIB_t_SoftBitBuf288	* inpSoftBitBuf,
		OUT ORILIB_t_SoftBitBuf432	* outSoftBitBuf
		){


	ORILIB_SoftDepuncturerThreeFourths(	
		inpSoftBitBuf->softBits,
		outSoftBitBuf->softBits,
		288
		);
}
