#ifndef SM4_AESNI_X4_H
#define SM4_AESNI_X4_H

#include"init_rkey.h"
#include <immintrin.h>
void SM4_AESNI_Encrypt_x4(uint8_t* plaintext, uint8_t* ciphertext, SM4_Key* sm4_key);

void SM4_AESNI_Decrypt_x4(uint8_t* ciphertext, uint8_t* plaintext, SM4_Key* sm4_key);

// 使用静态内联确保跨文件可见
static inline __m128i sm4_to_big_endian(__m128i x) {
    // 使用静态常量数组避免函数调用初始化问题
    static const uint8_t swap_mask_data[16] = {
        15, 14, 13, 12,  // 反转第4个32位
        11, 10, 9, 8,    // 反转第3个32位
        7, 6, 5, 4,      // 反转第2个32位
        3, 2, 1, 0       // 反转第1个32位
    };
    
    // 加载掩码（避免 _mm_set_epi8 在全局初始化）
    const __m128i mask = _mm_loadu_si128((const __m128i*)swap_mask_data);
    return _mm_shuffle_epi8(x, mask);
}
#endif // !SM4_AESNI_X4_H
