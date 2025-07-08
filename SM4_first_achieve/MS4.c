#include "MS4.h"
#include "func_T.h"
#include "basic_type.h"

word32* key_gen(word4 mk, word32* rk) {
    word K[4];
    for (int i = 0; i < 4; i++) {
        K[i] = FK[i] ^ mk[i];
    }
    for (int i = 0; i < 32; i++) {
        word temp = K[(i+1)%4] ^ K[(i+2)%4] ^ K[(i+3)%4] ^ CK[i];
        temp = func_T_extend_key(temp);
        word rk_i = K[i % 4] ^ temp;
        (*rk)[i] = rk_i;
        K[i % 4] = rk_i;
    }
    return rk;
}

word4* encrypt_block(word4* plaintext,word4 key,word4* cipertext)
{
    word32 rk={0};
    key_gen(key,&rk);
    word temp_plaintext[36];
    temp_plaintext[0]=(*plaintext)[0];
    temp_plaintext[1]=(*plaintext)[1];
    temp_plaintext[2]=(*plaintext)[2];
    temp_plaintext[3]=(*plaintext)[3];
    for(int i=4;i<36;i++)
    {
        temp_plaintext[i]=temp_plaintext[i-4]^func_T_encrypt(temp_plaintext[i-3]^temp_plaintext[i-2]^temp_plaintext[i-1]^rk[i-4]);
    }
    for(int i=0;i<4;i++)
    (*cipertext)[i]=temp_plaintext[35-i];
    return cipertext;
}

word4* decrypt_block(word4* cipertext,word4 key,word4* plaintext)
{
    word32 rk;
    key_gen(key,&rk);
    word temp_cipertext[36];
    temp_cipertext[0]=(*cipertext)[0];
    temp_cipertext[1]=(*cipertext)[1];
    temp_cipertext[2]=(*cipertext)[2];
    temp_cipertext[3]=(*cipertext)[3];
    for(int i=4;i<36;i++)
    {
        temp_cipertext[i]=temp_cipertext[i-4]^func_T_encrypt(temp_cipertext[i-3]^temp_cipertext[i-2]^temp_cipertext[i-1]^rk[35-i]);
    }
    for(int i=0;i<4;i++)
    (*plaintext)[i]=temp_cipertext[35-i];
    return plaintext;
}


