
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// SM4 S盒查表
static const uint8_t SM4_SBOX[256] = {
    0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,
    0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
    0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62,
    0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6,
    0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8,
    0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35,
    0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87,
    0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e,
    0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1,
    0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3,
    0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f,
    0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51,
    0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8,
    0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0,
    0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,
    0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48
};

// SM4线性变换L（优化：合并S盒输出和线性变换）
static uint32_t SM4_L(uint32_t x) {
    return x ^ ((x << 2) | (x >> 30)) ^ ((x << 10) | (x >> 22)) ^ 
           ((x << 18) | (x >> 14)) ^ ((x << 24) | (x >> 8));
}

// SM4轮函数（优化：合并查表和线性变换）
static uint32_t SM4_F(uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3, uint32_t rk) {
    uint32_t t = x1 ^ x2 ^ x3 ^ rk;
    
    // 合并S盒和线性变换L
    uint32_t s0 = SM4_SBOX[(t >> 24) & 0xFF];
    uint32_t s1 = SM4_SBOX[(t >> 16) & 0xFF];
    uint32_t s2 = SM4_SBOX[(t >>  8) & 0xFF];
    uint32_t s3 = SM4_SBOX[t & 0xFF];
    uint32_t s = (s0 << 24) | (s1 << 16) | (s2 << 8) | s3;
    
    return x0 ^ SM4_L(s);
}

// SM4密钥扩展
static void SM4_KeySchedule(const uint8_t *key, uint32_t *rk) {
    uint32_t k[4];
    for (int i = 0; i < 4; i++) {
        k[i] = ((uint32_t)key[4*i] << 24) | ((uint32_t)key[4*i+1] << 16) | 
               ((uint32_t)key[4*i+2] << 8) | key[4*i+3];
    }
    
    const uint32_t FK[4] = {0xA3B1BAC6, 0x56AA3350, 0x677D9197, 0xB27022DC};
    const uint32_t CK[32] = {
        0x00070E15, 0x1C232A31, 0x383F464D, 0x545B6269,
        0x70777E85, 0x8C939AA1, 0xA8AFB6BD, 0xC4CBD2D9,
        0xE0E7EEF5, 0xFC030A11, 0x181F262D, 0x343B4249,
        0x50575E65, 0x6C737A81, 0x888F969D, 0xA4ABB2B9,
        0xC0C7CED5, 0xDCE3EAF1, 0xF8FF060D, 0x141B2229,
        0x30373E45, 0x4C535A61, 0x686F767D, 0x848B9299,
        0xA0A7AEB5, 0xBCC3CAD1, 0xD8DFE6ED, 0xF4FB0209,
        0x10171E25, 0x2C333A41, 0x484F565D, 0x646B7279
    };
    
    k[0] ^= FK[0]; k[1] ^= FK[1]; k[2] ^= FK[2]; k[3] ^= FK[3];
    
    for (int i = 0; i < 32; i++) {
        uint32_t t = SM4_F(k[0], k[1], k[2], k[3], CK[i]);
        k[0] = k[1]; k[1] = k[2]; k[2] = k[3]; k[3] = t;
        rk[i] = t;
    }
}

// SM4加密单个块
static void SM4_EncryptBlock(const uint32_t *rk, const uint8_t *in, uint8_t *out) {
    uint32_t x[4];
    for (int i = 0; i < 4; i++) {
        x[i] = ((uint32_t)in[4*i] << 24) | ((uint32_t)in[4*i+1] << 16) | 
               ((uint32_t)in[4*i+2] << 8) | in[4*i+3];
    }
    
    for (int i = 0; i < 32; i += 4) {
        x[0] = SM4_F(x[0], x[1], x[2], x[3], rk[i]);
        x[1] = SM4_F(x[1], x[2], x[3], x[0], rk[i+1]);
        x[2] = SM4_F(x[2], x[3], x[0], x[1], rk[i+2]);
        x[3] = SM4_F(x[3], x[0], x[1], x[2], rk[i+3]);
    }
    
    // 最终反序
    uint32_t tmp = x[0]; x[0] = x[3]; x[3] = tmp;
    tmp = x[1]; x[1] = x[2]; x[2] = tmp;
    
    for (int i = 0; i < 4; i++) {
        out[4*i]   = (x[i] >> 24) & 0xFF;
        out[4*i+1] = (x[i] >> 16) & 0xFF;
        out[4*i+2] = (x[i] >> 8)  & 0xFF;
        out[4*i+3] = x[i] & 0xFF;
    }
}

// GCM模式：右移128位
static void right_shift(uint8_t *block) {
    for (int i = 0; i < 15; i++) {
        block[i] = (block[i] << 7) | (block[i+1] >> 1);
    }
    block[15] = block[15] << 7;
}

// GCM模式：Galois域乘法（优化：使用4位查表）
static void gfmul(const uint8_t *x, const uint8_t *y, uint8_t *z) {
    uint8_t v[16];
    uint8_t r = 0xE1; // 不可约多项式: x^128 + x^7 + x^2 + x + 1
    
    memcpy(v, y, 16);
    memset(z, 0, 16);
    
    for (int i = 0; i < 128; i++) {
        uint8_t byte_idx = i / 8;
        uint8_t bit_mask = 1 << (7 - (i % 8));
        
        if (x[byte_idx] & bit_mask) {
            for (int j = 0; j < 16; j++) {
                z[j] ^= v[j];
            }
        }
        
        uint8_t carry = v[15] & 0x01;
        right_shift(v);
        if (carry) {
            v[0] ^= r;
        }
    }
}

// GCM模式：初始化查表
static void ghash_precompute(uint8_t *H, uint8_t *T) {
    uint8_t tmp[16] = {0};
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            tmp[i] = (j << 4) | i; // 填充测试数据
            gfmul(H, tmp, T + 16*(16*i + j));
            tmp[i] = 0;
        }
    }
}

// GCM模式：使用查表加速的GHASH
static void ghash_table(const uint8_t *X, size_t len, const uint8_t *T, uint8_t *Y) {
    uint8_t tmp[16];
    memset(Y, 0, 16);
    
    for (size_t i = 0; i < len; i += 16) {
        for (int j = 0; j < 16; j++) {
            if (i + j < len) {
                tmp[j] = X[i + j] ^ Y[j];
            } else {
                tmp[j] = Y[j]; // 填充处理
            }
        }
        
        uint8_t *table_entry = T + 16 * (16 * (tmp[0] >> 4) + (tmp[0] & 0x0F));
        for (int j = 0; j < 16; j++) {
            Y[j] = table_entry[j];
        }
        
        for (int k = 1; k < 16; k++) {
            for (int j = 0; j < 16; j++) {
                tmp[j] = Y[j] ^ tmp[j];
            }
            table_entry = T + 16 * (16 * (tmp[k] >> 4) + (tmp[k] & 0x0F));
            for (int j = 0; j < 16; j++) {
                Y[j] = table_entry[j];
            }
        }
    }
}

// 增加计数器
static void inc32(uint8_t *counter) {
    uint32_t n = ((uint32_t)counter[12] << 24) | 
                 ((uint32_t)counter[13] << 16) | 
                 ((uint32_t)counter[14] << 8) | 
                 counter[15];
    n++;
    counter[12] = (n >> 24) & 0xFF;
    counter[13] = (n >> 16) & 0xFF;
    counter[14] = (n >> 8)  & 0xFF;
    counter[15] = n & 0xFF;
}

// SM4-GCM加密
void sm4_gcm_encrypt(
    const uint8_t *key, 
    const uint8_t *iv, size_t iv_len,
    const uint8_t *aad, size_t aad_len,
    const uint8_t *plaintext, size_t pt_len,
    uint8_t *ciphertext,
    uint8_t *tag, size_t tag_len
) {
    uint32_t rk[32];
    uint8_t H[16] = {0};
    uint8_t J0[16] = {0};
    uint8_t T[16 * 256] = {0}; // GHASH查表 (4位窗口)
    
    // 步骤1: 生成加密密钥
    SM4_KeySchedule(key, rk);
    
    // 步骤2: 计算H = E_K(0^128)
    SM4_EncryptBlock(rk, H, H);
    
    // 步骤3: 预计算GHASH表
    ghash_precompute(H, T);
    
    // 步骤4: 生成初始计数器J0
    if (iv_len == 12) {
        memcpy(J0, iv, 12);
        J0[15] = 0x01; // 标准推荐格式
    } else {
        // 对IV进行GHASH
        ghash_table(iv, iv_len, T, J0);
        uint8_t len_block[16];
        memset(len_block, 0, 16);
        len_block[15] = iv_len * 8;
        ghash_table(len_block, 16, T, J0);
    }
    
    // 步骤5: 加密数据 (CTR模式)
    uint8_t counter[16];
    memcpy(counter, J0, 16);
    inc32(counter); // 从J0+1开始
    
    for (size_t i = 0; i < pt_len; i += 16) {
        uint8_t keystream[16];
        SM4_EncryptBlock(rk, counter, keystream);
        inc32(counter);
        
        size_t len = (pt_len - i < 16) ? pt_len - i : 16;
        for (size_t j = 0; j < len; j++) {
            ciphertext[i + j] = plaintext[i + j] ^ keystream[j];
        }
    }
    
    // 步骤6: 计算GHASH
    uint8_t S[16] = {0};
    
    // 处理AAD
    ghash_table(aad, aad_len, T, S);
    
    // 处理密文
    ghash_table(ciphertext, pt_len, T, S);
    
    // 处理长度块
    uint8_t len_block[16];
    uint64_t aad_bits = aad_len * 8;
    uint64_t ct_bits = pt_len * 8;
    for (int i = 0; i < 8; i++) {
        len_block[i] = (aad_bits >> (56 - i*8)) & 0xFF;
        len_block[i+8] = (ct_bits >> (56 - i*8)) & 0xFF;
    }
    ghash_table(len_block, 16, T, S);
    
    // 步骤7: 计算认证标签
    uint8_t T0[16];
    SM4_EncryptBlock(rk, J0, T0);
    for (int i = 0; i < tag_len && i < 16; i++) {
        tag[i] = T0[i] ^ S[i];
    }
}

int main() {
    // 测试向量 (示例)
    uint8_t key[16] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10
    };
    
    uint8_t iv[12] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b
    };
    
    uint8_t aad[] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee};
    uint8_t plaintext[] = "Hello SM4-GCM! This is a test.";
    
    size_t pt_len = strlen((char*)plaintext);
    size_t aad_len = sizeof(aad);
    size_t tag_len = 16;
    
    uint8_t *ciphertext = malloc(pt_len);
    uint8_t *tag = malloc(tag_len);
    
    // 执行加密
    sm4_gcm_encrypt(key, iv, 12, aad, aad_len, 
                   plaintext, pt_len, ciphertext, tag, tag_len);
    
    // 输出结果
    printf("Plaintext:  %s\n", plaintext);
    printf("Ciphertext: ");
    for (size_t i = 0; i < pt_len; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\nTag: ");
    for (size_t i = 0; i < tag_len; i++) {
        printf("%02x", tag[i]);
    }
    printf("\n");
    
    free(ciphertext);
    free(tag);
    return 0;
}