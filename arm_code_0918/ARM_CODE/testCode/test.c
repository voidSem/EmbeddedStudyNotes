
#include <stdio.h>

int main(void)
{
	int  a = 0xffff ; 
	int  b = 0xffffffff ; 

	//算术右移    逻辑左移
	
	unsigned int c = 0xffff ; 
	unsigned int d = 0xffffffff ; 

	printf("a : %p c : %p \n" , a >> 4 , c >> 4);
	printf("b : %p d : %p \n" , b >> 24 , d >> 24);


	return 0 ;
}
