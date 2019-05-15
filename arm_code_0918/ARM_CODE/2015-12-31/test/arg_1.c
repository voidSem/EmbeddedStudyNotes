
#include <stdio.h>

void func(int a , ...);

int main(void)
{

    func(10,20,30,40,50,60,'A');

    return 0 ; 
}

void func(int a , ...)
{
    unsigned int *Addr = (unsigned int *)&a ; 
    Addr += 7 ; //固有偏移量   64位是有  32位是没有
    
    printf(" %d \n" , *Addr);
    Addr+=2 ;
    printf(" %d \n" , *Addr);
    Addr+=2 ;
    printf(" %d \n" , *Addr);

    Addr+=2 ;
    printf(" %d \n" , *Addr);

    Addr+=2 ;
    printf(" %d \n" , *Addr);

    char *p = Addr -2  ; 
    
    int i ;
    for(i = 0 ; i < 10 ; i++)
    {
        printf("p[%d] : %c \n" , i , p[i]);
    }
}

