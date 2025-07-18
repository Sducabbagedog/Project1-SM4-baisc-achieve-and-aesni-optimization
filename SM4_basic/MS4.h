#ifndef MS4_H
#define MS4_H
#include "basic_type.h"
void encrypt_block(word4 plaintext,word4 key,word4 cipertext);
void decrypt_block(word4 cipertext,word4 key,word4 plaintext);
void SM4_enc_file(char * file_name,word4 key,char * ciper_file_name);
void SM4_dec_file(char * file_name,word4 key,char * plain_file_name);
#endif