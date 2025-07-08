#ifndef MS4_H
#define MS4_H
#include "basic_type.h"
word4* encrypt_block(word4* plaintext,word4 key,word4* cipertext);
word4* decrypt_block(word4* cipertext,word4 key,word4* plaintext);
#endif