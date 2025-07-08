#include "func_T.h"
#include "basic_type.h"

word loop_shift_left(word a,int n)
{
    return (a>>(32-n))|(a<<n);
}

word loop_shift_right(word a,int n)
{
    return a=(a<<(32-n))|(a>>n);
}

word func_T_extend_key(word k) {
    byte a[4];
    a[0]=(k>>24)&0xff;
    a[1]=(k>>16)&0xff;
    a[2]=(k>>8)&0xff;
    a[3]=(k)&0xff;
    /*cross the sbox*/
    
    for(int i=0;i<4;i++)
    {
        a[i]=sbox[(a[i]>>4)][a[i]&0x0f];
    }
    word b=(((word)a[0])<<24)+(((word)a[1])<<16)+(((word)a[2])<<8)+(((word)a[3]));
    //return (b<<13)^(b)^(b<<23);
    return loop_shift_left(b,13)^b^loop_shift_left(b,23);
}

word func_T_encrypt(word k) {
    byte a[4];
    a[0]=(k>>24)&0xff;
    a[1]=(k>>16)&0xff;
    a[2]=(k>>8)&0xff;
    a[3]=(k)&0xff;
    /*cross the sbox*/
    
    for(int i=0;i<4;i++)
    {
        a[i]=sbox[(a[i]>>4)][a[i]&0x0f];
    }
    word b=(((word)a[0])<<24)+(((word)a[1])<<16)+(((word)a[2])<<8)+(((word)a[3]));
    //return (b<<2)^(b<<10)^(b<<18)^(b<<24);
    return loop_shift_left(b,2)^loop_shift_left(b,10)^loop_shift_left(b,18)^loop_shift_left(b,24)^b;
}



