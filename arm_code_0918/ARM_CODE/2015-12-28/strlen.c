
#include <stdio.h>


int main(void)
{
	char *p = "hello world" ; 

	int len ; 
	
	__asm__ __volatile__ (

	"mov	r0 , %1   \n"
	"mov	r2 , #0   \n"
	
	"again:     \n"

	"ldrb	r1 , [r0] \n"

	"cmp	r1 , #0   \n"

	"addne	r2 , r2 , #1   \n"
	"addne	r0 , r0 , #1   \n"
	"bne	again     \n"

	"mov	%0 , r2   \n"

	:"=r"(len)
	:"r"(p)
	:"r0","r1","r2"
	);

	printf("len: %d \n" , len);
	return 0 ; 
}
