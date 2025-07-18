#ifndef SM4_BOX_AESENCLAST_INTEL
#define SM4_BOX_AESENCLAST_INTEL
#include<stdint.h>
#include<immintrin.h>

__m128i sm4_box_aesenclast_intel(uint32_t rk,   __m128i t0, __m128i t1, __m128i t2, __m128i t3);

#endif