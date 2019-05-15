
#include <stdio.h>

int main(void)
{

	int a = 10 ; 
	int b = 20 ; 
	
	int add ; 
	int sub ; 
	int mul ; 

	__asm__ __volatile__ (

	//加载 
	"mov	r0 , %3   \n"
	"mov	r1 , %4   \n"

	//运算
	"add	r2 , r0 , r1   \n"
	"sub	r3 , r1 , r0   \n"
	"mul	r4 , r0 , r1   \n"

	//输出
	"mov	%0 , r2   \n"
	"mov	%1 , r3   \n"
	"mov	%2 , r4   \n"

	:"=r"(add),"=r"(sub),"=r"(mul)
	:"r"(a),"r"(b)
	:"r0","r1","r2","r3","r4"
	);


	printf("a : %d b : %d add:%d sub:%d mul:%d \n" , 
	a , b , add , sub , mul);

	return 0 ;
}
