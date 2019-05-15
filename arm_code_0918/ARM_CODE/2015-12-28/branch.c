
#include <stdio.h>

int main(void)
{

	unsigned int sum ; 

	__asm__ __volatile__ (

	"mov	r0 , #1   \n"
	"mov	r0 , #2   \n"

	"b		next      \n"

	"mov	r0 , #3   \n"

	"next:			  \n"

	"mov	%0 , r0   \n"
	:"=r"(sum)
	::"r0"
	);
	printf("sum : %d \n" , sum);

	return 0 ; 
}
