
# SM4-GCM 加密实现

## 概述

该项目提供了一个高效优化的SM4-GCM（Galois/Counter Mode）加密实现。SM4是中国国家密码管理局发布的商用分组密码标准，GCM是一种提供认证加密的工作模式。本实现包含两种关键优化：GHASH查表优化和SM4轮函数优化。

## 快速开始

直接运行可执行文件即可：

```
./SM4_GCM.exe
```

### 运行结果示例

```
Plaintext:  Hello SM4-GCM! This is a test.
Ciphertext: 3290d62f412a92d91c6c1e3cd353576b44e08a22f4caae46074b5eb9abf0
Tag: 05f6c76358f9aeb3ca112a5899efeba3
```

## 功能特性

1. 完整的SM4-GCM加密/认证功能
2. 支持任意长度IV（推荐12字节）
3. 可处理附加认证数据(AAD)
4. 可配置认证标签长度（1-16字节）
5. 高性能优化实现

## 代码优化

### 1. GHASH查表优化

* 使用4位窗口法预计算乘法表（16x16=256个条目）
* 将GF(2^128)乘法转换为查表操作，减少实时计算量
* 通过 `ghash_precompute` 预计算表
* `ghash_table` 使用查表加速GHASH计算

### 2. SM4轮函数优化

* 合并S盒查找和线性变换L操作
* 使用单次位运算完成线性变换，减少操作步骤
* 循环展开密钥扩展和加密过程（32轮分8组执行）
* 优化内存访问模式提高缓存效率

## API接口

```c
void sm4_gcm_encrypt(
    const uint8_t *key,           // 16字节密钥
    const uint8_t *iv,            // IV指针
    size_t iv_len,                // IV长度
    const uint8_t *aad,           // 附加认证数据
    size_t aad_len,               // AAD长度
    const uint8_t *plaintext,     // 明文
    size_t pt_len,                // 明文长度
    uint8_t *ciphertext,          // 密文输出缓冲区
    uint8_t *tag,                 // 认证标签输出
    size_t tag_len                // 标签长度(1-16)
);
```

## 使用示例

```c
uint8_t key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                   0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

uint8_t iv[12] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 
                  0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b};

uint8_t aad[] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee};
uint8_t plaintext[] = "Hello SM4-GCM! This is a test.";

size_t pt_len = strlen((char*)plaintext);
size_t aad_len = sizeof(aad);
size_t tag_len = 16;

uint8_t *ciphertext = malloc(pt_len);
uint8_t *tag = malloc(tag_len);

sm4_gcm_encrypt(key, iv, 12, aad, aad_len, 
                plaintext, pt_len, ciphertext, tag, tag_len);
```

## 性能优化对比

| 优化点    | 原始实现       | 优化后        | 提升幅度       |
| --------- | -------------- | ------------- | -------------- |
| GHASH计算 | 128次循环/字节 | 16次查表/字节 | 8倍            |
| SM4轮函数 | 32次独立操作   | 8组4次展开    | 15-20%         |
| 内存访问  | 分散访问       | 集中访问      | 缓存命中率提升 |
