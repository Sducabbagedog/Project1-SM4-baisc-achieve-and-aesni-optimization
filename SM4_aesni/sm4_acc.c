#include "sm4_aesin_x4.h"
#include "init_rkey.h"
#include <stdlib.h>
#include <stdio.h>
#include<string.h>

int main(int argc,char * argv[])
{
    if(argc!=5)
    {
        printf("Usage: <-E/-D> <plaintext> <key> <ciphertext>  \n");
        exit(-1);
    }


    FILE *fp = fopen(argv[3], "rb");
    if (fp == NULL) {
        perror("Error opening file:\n");
        exit(-1);
    }

    uint8_t key[16];
    int tmp=fread(key, sizeof(uint8_t), 16, fp);
    fclose(fp);
    if (tmp!=16) {
        printf("Error reading file,we cannot get 16 byte key\n");
        exit(-1);
    }

    SM4_Key sm4_key;
    SM4_KeyInit((uint8_t*)key, &sm4_key);

    FILE *fp2 = fopen(argv[2], "rb");//IN
    if (fp2 == NULL) {
        perror("Error opening input file: \n");
        exit(-1);
    }

    FILE *fp3 = fopen(argv[4], "wb");//OUT
    if (fp3 == NULL) {
        perror("Error opening file:\n");
        exit(-1);
    }

    if (strcmp(argv[1], "-E") == 0) {
        int count=0;
        uint8_t in[64];
        uint8_t out[64];
        count=fread(in,sizeof(uint8_t),64,fp2);
        while(count==64)
        {
            SM4_AESNI_Encrypt_x4(in, out, &sm4_key);
            fwrite(out,sizeof(uint8_t),64,fp3);
            count=fread(in,sizeof(uint8_t),64,fp2);
        }
        if(count<0)
        {
            perror(" fread fail:\n");
            exit(-1);
        }
        else if(count<64 && count>0)
        {
            for(int i=count;i<64;i++)
            {
                in[i]=0x00;
                SM4_AESNI_Encrypt_x4(in, out, &sm4_key);
                fwrite(out,sizeof(uint8_t),64,fp3);
            }
        }
        fclose(fp2);
        fclose(fp3);
        
    } else if (strcmp(argv[1], "-D") == 0) {
        int count=0;
        uint8_t in[64];
        uint8_t out[64];
        count=fread(in,sizeof(uint8_t),64,fp2);
        while(count==64)
        {
            SM4_AESNI_Decrypt_x4(in, out, &sm4_key);
            fwrite(out,sizeof(uint8_t),64,fp3);
            count=fread(in,sizeof(uint8_t),64,fp2);
        }
        if(count<0)
        {
            perror(" fread fail:\n");
            exit(-1);
        }
        else if(count<64 && count>0)
        {
            for(int i=count;i<64;i++)
            {
                in[i]=0x00;
                SM4_AESNI_Decrypt_x4(in, out, &sm4_key);
                fwrite(out,sizeof(uint8_t),64,fp3);
            }
        }
        fclose(fp2);
        fclose(fp3);
        
    }



}