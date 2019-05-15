
#include <stdio.h>
#include <stdarg.h>

void func(int a , ...);

int main(void)
{

    func(10,20,30,40,50,60,70);

    1;
    1;
    2;
    3;
    4;
    5;


    return 0 ; 
}

void func(int a , ...)
{
    va_list ap ; 

    va_start(ap ,  a ) ;

    int tmp = va_arg(ap , int);

    printf("tmp : %d \n" , tmp);

    tmp = va_arg(ap , int);
    printf("tmp : %d \n" , tmp);

    tmp = va_arg(ap , int);
    printf("tmp : %d \n" , tmp);

    tmp = va_arg(ap , int);
    printf("tmp : %d \n" , tmp);

    tmp = va_arg(ap , int);
    printf("tmp : %d \n" , tmp);

    va_end(ap);
}

