#include "MS4.h"
#include <stdio.h>

int main()
{
    word4 plaintext={0x01234567,0x89ABCDEF,0xFEDCBA98,0X76543210};
    word4 plaintext_gen={0};
    word4 key={0x01234567,0x89ABCDEF,0xFEDCBA98,0X76543210};
    word4 cipertext={0X681EDF34,0XD206965E,0X86B3E94F,0X536E4246};
    word4 cipertext_gen={0};
    encrypt_block(&plaintext,key,&cipertext_gen);
    printf("cipertext_gen is: %llx,%llx,%llx,%llx \n",cipertext_gen[0],cipertext_gen[1],cipertext_gen[2],cipertext_gen[3]);
    printf("cipertext is : %llx,%llx,%llx,%llx \n",cipertext[0],cipertext[1],cipertext[2],cipertext[3]);
    for(int i=0;i<4;i++)
    {
        if (cipertext[i]!=cipertext_gen[i])
        {
            printf("encrypt fail\n");
            break;
        }
    }

    decrypt_block(&cipertext,key,&plaintext_gen);
    printf("plaintext_gen is : %llx,%llx,%llx,%llx \n",plaintext_gen[0],plaintext_gen[1],plaintext_gen[2],plaintext_gen[3]);
    printf("plaintext_gen is : %llx,%llx,%llx,%llx \n",plaintext[0],plaintext[1],plaintext[2],plaintext[3]);
    for(int i=0;i<4;i++)
    {
        if (plaintext_gen[i]!=plaintext[i])
        {
            printf("decrypt fail\n");
            break;
        }
    }

    // //test key_gen
    // word4 key={0x01234567,0x89ABCDEF,0xFEDCBA98,0X76543210};
    // word32 roundkey={0};
    // key_gen(key,&roundkey);
    
}