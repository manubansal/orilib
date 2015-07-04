/**
Atomix project, ORILIB_SoftBranchMetricsR1by2_algorithms.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/



#include <osl/inc/or_types.h>
#include <osl/inc/swpform.h>

void ORILIB_SoftBranchMetricsR1by2_calculate(
	IN  BitsSoft const * const depunctured_bits,
	OUT BitsSoft * const branch_metrics,
	IN  uint32_t FrameLength 
	){

    int32_t   i;
    int64_t   s7s6s5s4s3s2s1s0, temp_ms3ps2ms1ps0, temp_ms7ps6ms5ps4;
    uint32_t  ms3ps2ms1ps0, ms7ps6ms5ps4, bm3210, bm7654;
    int64_t   * restrict in_ptr1 = ( int64_t * )depunctured_bits;
    int64_t   * restrict out_ptr1 = ( int64_t * )branch_metrics;
	
    Int32 	m1p1m1p1 = 0xff01ff01;

    for ( i = 0; i < FrameLength; i += 4 ) {	
        s7s6s5s4s3s2s1s0       = _amem8(( void * ) in_ptr1++);
          
        temp_ms3ps2ms1ps0 = _mpysu4ll(m1p1m1p1, _loll(s7s6s5s4s3s2s1s0));     
        ms3ps2ms1ps0 = _packl4(_hill(temp_ms3ps2ms1ps0), _loll(temp_ms3ps2ms1ps0));
        bm3210 = _sub4(ms3ps2ms1ps0,_swap4(_loll(s7s6s5s4s3s2s1s0)));
        
        temp_ms7ps6ms5ps4 = _mpysu4ll(m1p1m1p1, _hill(s7s6s5s4s3s2s1s0));     
        ms7ps6ms5ps4 = _packl4(_hill(temp_ms7ps6ms5ps4), _loll(temp_ms7ps6ms5ps4));
        bm7654 = _sub4( ms7ps6ms5ps4,_swap4(_hill(s7s6s5s4s3s2s1s0)));

        _amem8(( void * ) out_ptr1++) = _itoll(bm7654, bm3210);

    }
}

