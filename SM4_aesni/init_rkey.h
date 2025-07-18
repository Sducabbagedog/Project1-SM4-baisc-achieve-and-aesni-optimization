#ifndef INIT_RKEY
#define INIT_RKEY

#include <stdint.h>

/**
 * @brief SM4 密钥
 */
typedef struct _SM4_Key {
    uint32_t rk[32];//32轮密钥
} SM4_Key;

/**
 * @brief 初始化 SM4 轮密钥
 * @param key 128bit长度密钥
 * @param sm4_key SM4 密钥
 */
void SM4_KeyInit(uint8_t* key, SM4_Key* sm4_key);

#endif 
