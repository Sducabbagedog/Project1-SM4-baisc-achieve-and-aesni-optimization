#include "sm4_aesin_x4.h"
#include <immintrin.h>
#include "sm4_box_aesenclast_intel.h"

static void SM4_AESNI_do(uint8_t* in, uint8_t* out, SM4_Key* sm4_key, int enc);

void SM4_AESNI_Encrypt_x4(uint8_t* plaintext, uint8_t* ciphertext,SM4_Key* sm4_key) 
{
SM4_AESNI_do(plaintext, ciphertext, sm4_key, 0);
}

void SM4_AESNI_Decrypt_x4(uint8_t* ciphertext, uint8_t* plaintext,SM4_Key* sm4_key) 
{
    SM4_AESNI_do(ciphertext, plaintext, sm4_key, 1);
}

#define MM_PACK0_EPI32(a, b, c, d) \
    _mm_unpacklo_epi64(_mm_unpacklo_epi32(a, b), _mm_unpacklo_epi32(c, d))
#define MM_PACK1_EPI32(a, b, c, d) \
    _mm_unpackhi_epi64(_mm_unpacklo_epi32(a, b), _mm_unpacklo_epi32(c, d))
#define MM_PACK2_EPI32(a, b, c, d) \
    _mm_unpacklo_epi64(_mm_unpackhi_epi32(a, b), _mm_unpackhi_epi32(c, d))
#define MM_PACK3_EPI32(a, b, c, d) \
    _mm_unpackhi_epi64(_mm_unpackhi_epi32(a, b), _mm_unpackhi_epi32(c, d))

#define MM_XOR2(a, b) _mm_xor_si128(a, b)
#define MM_XOR3(a, b, c) MM_XOR2(a, MM_XOR2(b, c))
#define MM_XOR4(a, b, c, d) MM_XOR2(a, MM_XOR3(b, c, d))
#define MM_XOR5(a, b, c, d, e) MM_XOR2(a, MM_XOR4(b, c, d, e))
#define MM_XOR6(a, b, c, d, e, f) MM_XOR2(a, MM_XOR5(b, c, d, e, f))
// 循环移位宏
#define MM_ROTL_EPI32(a, n) MM_XOR2(_mm_slli_epi32(a,n), _mm_srli_epi32(a,32-n))

static void SM4_AESNI_do(uint8_t* in, uint8_t* out, SM4_Key* sm4_key, int enc) {
    __m128i X[4], Tmp[4];
    // Load Data
    Tmp[0] = _mm_loadu_si128((const __m128i*)in + 0);
    Tmp[1] = _mm_loadu_si128((const __m128i*)in + 1);
    Tmp[2] = _mm_loadu_si128((const __m128i*)in + 2);
    Tmp[3] = _mm_loadu_si128((const __m128i*)in + 3);
    
    // Pack Data
    X[0] = MM_PACK0_EPI32(Tmp[0], Tmp[1], Tmp[2], Tmp[3]);
    X[1] = MM_PACK1_EPI32(Tmp[0], Tmp[1], Tmp[2], Tmp[3]);
    X[2] = MM_PACK2_EPI32(Tmp[0], Tmp[1], Tmp[2], Tmp[3]);
    X[3] = MM_PACK3_EPI32(Tmp[0], Tmp[1], Tmp[2], Tmp[3]);
    // // Shuffle Endian
    X[0]=sm4_to_big_endian(X[0]);
    X[1]=sm4_to_big_endian(X[1]);
    X[2]=sm4_to_big_endian(X[2]);
    X[3]=sm4_to_big_endian(X[3]);
    // Loop
    for (int i = 0; i < 32; i++) {
        uint32_t rk=((enc==0)?sm4_key->rk[i] : sm4_key->rk[31 - i]);
        Tmp[0]=sm4_box_aesenclast_intel(rk,X[0],X[1],X[2],X[3]);
        Tmp[0] = MM_XOR6(X[0], Tmp[0], MM_ROTL_EPI32(Tmp[0], 2),MM_ROTL_EPI32(Tmp[0], 10), MM_ROTL_EPI32(Tmp[0], 18),MM_ROTL_EPI32(Tmp[0], 24));
        X[0] = X[1];
        X[1] = X[2];
        X[2] = X[3];
        X[3] = Tmp[0];
    }
    // // Shuffle Endian
    X[0]=sm4_to_big_endian(X[0]);
    X[1]=sm4_to_big_endian(X[1]);
    X[2]=sm4_to_big_endian(X[2]);
    X[3]=sm4_to_big_endian(X[3]);
    // Pack and Store
    _mm_storeu_si128((__m128i*)out + 0, MM_PACK0_EPI32(X[3], X[2], X[1], X[0]));
    _mm_storeu_si128((__m128i*)out + 1, MM_PACK1_EPI32(X[3], X[2], X[1], X[0]));
    _mm_storeu_si128((__m128i*)out + 2, MM_PACK2_EPI32(X[3], X[2], X[1], X[0]));
    _mm_storeu_si128((__m128i*)out + 3, MM_PACK3_EPI32(X[3], X[2], X[1], X[0]));
}

