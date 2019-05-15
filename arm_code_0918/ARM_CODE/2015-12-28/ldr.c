
#include <stdio.h>

int main(void)
{
	
	int a = 200 ; 
	int sum = 0 ; 

	__asm__ __volatile__ (

	"mov	r0 , %0   \n"
	"mov	r1 , %1   \n"


	"ldr	r2 , [r0] \n"  //取值

	"str	r2 , [r1] \n"  //写值

	::"r"(&a),"r"(&sum)	
	:"r0","r1"
	);


	printf("sum : %d \n" , sum);

	return 0 ; 
}
