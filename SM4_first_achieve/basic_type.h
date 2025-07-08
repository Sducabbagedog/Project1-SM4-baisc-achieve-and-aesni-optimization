#ifndef BASIC_TYPE_H
#define BASIC_TYPE_H
#include <stdint.h>
#include <stdio.h>
typedef uint8_t byte;
typedef unsigned int word;

typedef word word4[4];
typedef word word32[32];

extern const word4 FK;
extern const word32 CK;
extern const byte sbox[16][16];

#endif