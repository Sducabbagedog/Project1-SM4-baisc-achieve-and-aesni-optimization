#include "MS4.h"
#include "func_T.h"
#include "basic_type.h"
#include <errno.h>
#include <assert.h>
#include <stdlib.h>

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

void encrypt_block(word4 plaintext,word4 key,word4 cipertext)
{
    word32 rk={0};
    key_gen(key,&rk);
    word temp_plaintext[36];
    temp_plaintext[0]=plaintext[0];
    temp_plaintext[1]=plaintext[1];
    temp_plaintext[2]=plaintext[2];
    temp_plaintext[3]=plaintext[3];
    for(int i=4;i<36;i++)
    {
        temp_plaintext[i]=temp_plaintext[i-4]^func_T_encrypt(temp_plaintext[i-3]^temp_plaintext[i-2]^temp_plaintext[i-1]^rk[i-4]);
    }
    for(int i=0;i<4;i++)
    cipertext[i]=temp_plaintext[35-i];
    
}

void decrypt_block(word4 cipertext,word4 key,word4 plaintext)
{
    word32 rk;
    key_gen(key,&rk);
    word temp_cipertext[36];
    temp_cipertext[0]=cipertext[0];
    temp_cipertext[1]=cipertext[1];
    temp_cipertext[2]=cipertext[2];
    temp_cipertext[3]=cipertext[3];
    for(int i=4;i<36;i++)
    {
        temp_cipertext[i]=temp_cipertext[i-4]^func_T_encrypt(temp_cipertext[i-3]^temp_cipertext[i-2]^temp_cipertext[i-1]^rk[35-i]);
    }
    for(int i=0;i<4;i++)
    plaintext[i]=temp_cipertext[35-i];
}


long long int getFileSize(FILE* file) {
    fseek(file, 0L, SEEK_END);  // 将文件指针定位到文件末尾
    long long int size = ftell(file);    // 获取文件指针的当前位置（即文件大小）
    fseek(file, 0L, SEEK_SET);  // 将文件指针恢复到文件开头
    return size;
}

void SM4_enc_file(char * file_name,word4 key,char * ciper_file_name)
{
    FILE *fp=fopen(file_name,"rb");
    if (fp==NULL)
    {
        perror("open file error :");
        exit(-1);
    }
    FILE *fp2=fopen(ciper_file_name,"wb");
    if(fp2==NULL)
    {
        perror("open out_file error :");
        fclose(fp);
        exit(-1);
    }
    word4 tmp_plaintext;
    word4 tmp_cipertext;
    long long int count=0;
    int ret=0;
    while(1)
    {   ret=fread(tmp_plaintext,sizeof(byte),16,fp);
        if (ret!=16)
        {
            if (ret<0)
            {
                perror("read file error :");
                fclose(fp);
                fclose(fp2);
                exit(-1);
            }
            if(ret==0)
            {
                fclose(fp);
                fclose(fp2);
                break;
            }
            if(ret<16)
            {
                for(int i=ret;i<16;i++)
                {
                    ((byte *)tmp_plaintext)[i]=0x00;
                }
                encrypt_block(tmp_plaintext,key,tmp_cipertext);
                fwrite(tmp_cipertext,sizeof(byte),16,fp2);
                count+=ret;
                fclose(fp);
                fclose(fp2);
                break;
            }
        }
        if(ret==16)
        {
        encrypt_block(tmp_plaintext,key,tmp_cipertext);
        fwrite(tmp_cipertext,sizeof(byte),16,fp2);
        count+=ret;
        }
    }
}



void SM4_dec_file(char * file_name,word4 key,char * plain_file_name)
{
    FILE *fp=fopen(file_name,"rb");
    if (fp==NULL)
    {
        perror("open file error :");
        exit(-1);
    }
    FILE *fp2=fopen(plain_file_name,"wb");
    if(fp2==NULL)
    {
        perror("open out_file error :");
        exit(-1);
    }
    word4 tmp_cipertext;
    word4 tmp_plaintext;
    long long int count=0;
    int ret=0;
    while(1)
    {   ret=fread(tmp_cipertext,sizeof(byte),16,fp);
        if (ret!=16)
        {
            if (ret<0)
            {
                perror("read file error :");
                fclose(fp);
                fclose(fp2);
                exit(-1);
            }
            if(ret==0)
            {
                fclose(fp);
                fclose(fp2);
                break;
            }
            if(ret<16)
            {
                for(int i=ret;i<16;i++)
                {
                    ((byte *)tmp_plaintext)[i]=0x00;
                }
                decrypt_block(tmp_cipertext,key,tmp_plaintext);
                fwrite(tmp_plaintext,sizeof(byte),16,fp2);
                count+=ret;
                fclose(fp);
                fclose(fp2);
                break;
            }
        }
        if(ret==16)
        {
        decrypt_block(tmp_cipertext,key,tmp_plaintext);
        fwrite(tmp_plaintext,sizeof(byte),16,fp2);
        count+=ret;
        }
    }
}


























