
#include <stdio.h>

int main(void)
{
    //逗号表达式　
    int a = (1,2,3,4,5,6,7,8,9);

    printf("a : %d \n" , a );


    //语句块表达式
    int b = ({ int a ;  int b ; a = 200 ;1 ; 2 ; 3 ; 4 ; 5 ; 6 ;a ; });

    printf("b : %d \n" , b);

    
    printf("c : %d \n" , 33.33);

    float c = 33.33 ; 

    printf("c : %d \n" , (int)c);

    return 0 ; 
}
