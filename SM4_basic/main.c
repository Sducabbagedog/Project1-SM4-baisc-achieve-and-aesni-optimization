#include "MS4.h"
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>

// interface main
int main(int argc, char const *argv[])
{
    //要求4个参数
    if (argc!=5)
    {
        printf("argc num error\n");
        exit(-1);
    }
    //读取key
    FILE * fp=fopen(argv[3],"r");
    word4 key;
    if (fp!=NULL)
    {
        fread(key,sizeof(word4),1,fp);
        fclose(fp);
    }
    else
    {
        perror("open file error :");
        exit(-1);
    }  
    //printf("argc[1] is %s",argv[1]);
    //查看是加密还是解密
    if(strcmp(argv[1],"-E")==0)
    {
        SM4_enc_file(argv[2],key,argv[4]);
    }
    else if(strcmp(argv[1],"-D")==0)
    {
        SM4_dec_file(argv[2],key,argv[4]);
    }
    else
    {
        printf("please input a correct mod, -D/-E.\n");
    }

}



// //test example
// int main()
// {
    
//     word4 plaintext={0x01234567,0x89ABCDEF,0xFEDCBA98,0X76543210};
//     word4 plaintext_gen={0};
//     word4 key={0x01234567,0x89ABCDEF,0xFEDCBA98,0X76543210};
//     word4 cipertext={0X681EDF34,0XD206965E,0X86B3E94F,0X536E4246};
//     word4 cipertext_gen={0};
//     encrypt_block(plaintext,key,cipertext_gen);
//     printf("cipertext_gen is: %llx,%llx,%llx,%llx \n",cipertext_gen[0],cipertext_gen[1],cipertext_gen[2],cipertext_gen[3]);
//     printf("cipertext is : %llx,%llx,%llx,%llx \n",cipertext[0],cipertext[1],cipertext[2],cipertext[3]);
//     for(int i=0;i<4;i++)
//     {
//         if (cipertext[i]!=cipertext_gen[i])
//         {
//             printf("encrypt fail\n");
//             break;
//         }
//     }

//     decrypt_block(cipertext,key,plaintext_gen);
//     printf("plaintext_gen is : %llx,%llx,%llx,%llx \n",plaintext_gen[0],plaintext_gen[1],plaintext_gen[2],plaintext_gen[3]);
//     printf("plaintext_gen is : %llx,%llx,%llx,%llx \n",plaintext[0],plaintext[1],plaintext[2],plaintext[3]);
//     for(int i=0;i<4;i++)
//     {
//         if (plaintext_gen[i]!=plaintext[i])
//         {
//             printf("decrypt fail\n");
//             break;
//         }
//     }


//     // //test key_gen
//     // word4 key={0x01234567,0x89ABCDEF,0xFEDCBA98,0X76543210};
//     // word32 roundkey={0};
//     // key_gen(key,&roundkey);

    
//}

    
