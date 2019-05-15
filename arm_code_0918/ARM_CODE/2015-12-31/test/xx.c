
#include <stdio.h>

int main(void)
{
    int a  = 100 ;   
    typeof(a)  b = 200 ; 

    
    a = (a + sizeof(a)-1) & (~(sizeof(a)-1)) ; 




    return 0 ; 
}
