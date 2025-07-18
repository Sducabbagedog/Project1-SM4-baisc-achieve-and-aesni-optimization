#include"sm4_box_aesenclast_intel.h"
//const uint64_t a1l[2] = { 0xdcf84460b3972b0f, 0xb6922e0ad9fd4165 };
//const uint64_t a1h[2] = { 0x64ad03cae42d834a, 0x2ee74980ae67c900 };
//const uint64_t a2l[2] = { 0x48c2a32957ddbc36, 0xad2746ccb23859d3 };
//const uint64_t a2h[2] = { 0x134307579aca8ede, 0xcd9dd98944145000 };

//uint8_t a1l[16] = { 0x65, 0x41, 0xfd, 0xd9, 0xa, 0x2e, 0x92, 0xb6, 0xf, 0x2b, 0x97, 0xb3, 0x60, 0x44, 0xf8, 0xdc, };
//uint8_t a1h[16] = { 0x0, 0xc9, 0x67, 0xae, 0x80, 0x49, 0xe7, 0x2e, 0x4a, 0x83, 0x2d, 0xe4, 0xca, 0x3, 0xad, 0x64, };
//uint8_t a2l[16] = { 0x4e, 0xc4, 0xa5, 0x2f, 0x51, 0xdb, 0xba, 0x30, 0xab, 0x21, 0x40, 0xca, 0xb4, 0x3e, 0x5f, 0xd5, };
//uint8_t a2h[16] = { 0x9d, 0xcd, 0x89, 0xd9, 0x14, 0x44, 0x0, 0x50, 0x43, 0x13, 0x57, 0x7, 0xca, 0x9a, 0xde, 0x8e, };


const uint8_t a1l[16] = { 0x65, 0x41, 0xfd, 0xd9, 0xa, 0x2e, 0x92, 0xb6, 0xf, 0x2b, 0x97, 0xb3, 0x60, 0x44, 0xf8, 0xdc, };
const uint8_t a1h[16] = { 0x0, 0xc9, 0x67, 0xae, 0x80, 0x49, 0xe7, 0x2e, 0x4a, 0x83, 0x2d, 0xe4, 0xca, 0x3, 0xad, 0x64, };
const uint8_t a2l[16] = { 0x2f, 0xa5, 0xc4, 0x4e, 0x30, 0xba, 0xdb, 0x51, 0xca, 0x40, 0x21, 0xab, 0xd5, 0x5f, 0x3e, 0xb4, };
const uint8_t a2h[16] = { 0x0, 0x50, 0x14, 0x44, 0x89, 0xd9, 0x9d, 0xcd, 0xde, 0x8e, 0xca, 0x9a, 0x57, 0x7, 0x43, 0x13, };


__m128i sm4_box_aesenclast_intel(uint32_t rk,   __m128i t0, __m128i t1, __m128i t2, __m128i t3) {
    // 设置轮密钥 (4个相同的rk值)
    __m128i const_0f = _mm_set1_epi8(0x0F);
    __m128i intelenckey = _mm_set1_epi8(0x00); // 0x63 重复 16 次
    __m128i a1_low = _mm_loadu_si128((__m128i*)a1l);
    __m128i a1_high = _mm_loadu_si128((__m128i*)a1h);
    __m128i a2_low = _mm_loadu_si128((__m128i*)a2l);
    __m128i a2_high = _mm_loadu_si128((__m128i*)a2h);

    __m128i rk128 = _mm_set1_epi32(rk);

    // 状态组合: x = t1 ^ t2 ^ t3 ^ rk
    __m128i x = _mm_xor_si128(t1, t2);
    x = _mm_xor_si128(x, t3);
    x = _mm_xor_si128(x, rk128);

    // 前向变换 (SM4域 → AES域)
    __m128i y = _mm_and_si128(x, const_0f);          // 提取低4位
    y = _mm_shuffle_epi8(a1_low, y);                   // 用低4位查a1l表
    x = _mm_srli_epi64(x, 4);                       // 右移4位
    x = _mm_and_si128(x, const_0f);                 // 提取高4位
    x = _mm_xor_si128(_mm_shuffle_epi8(a1_high, x), y); // 查a1h表并异或

    // AES S盒应用 + 行移位修正
    x = _mm_aesenclast_si128(x, intelenckey);        // AES-NI指令
    //x = _mm_shuffle_epi8(x, intelmaskSrows);         // 行移位修正

    // 逆向变换 (AES域 → SM4域) + 线性变换融合
    y = _mm_and_si128(x, const_0f);                 // 提取低4位
    y = _mm_shuffle_epi8(a2_low, y);                   // 用低4位查a2l表
    x = _mm_srli_epi64(x, 4);                       // 右移4位
    x = _mm_and_si128(x, const_0f);                 // 提取高4位
    x = _mm_xor_si128(_mm_shuffle_epi8(a2_high, x), y); // 查a2h表并异或

    return x;
}
