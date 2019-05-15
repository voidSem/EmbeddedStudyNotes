
#include <stdio.h>

int main(void)
{
	unsigned int sum  ;

	__asm__ __volatile__ (
	//S  <c> <q>
	"mov	r0 , #1   \n"
	"mov	r1 , #2   \n"

	"cmp	r0 , r1   \n"  //改动cpsr  Status
	
	//ne  eq  gt  ge  lt le       equal =
	//!=  ==  >   >=  <  <=
	"movne	%0 , #100 \n"
	"moveq	%0 , #200 \n"

	:"=r"(sum)
	::"r0","r1"
	);
	
	printf("sum : %d \n" , sum);

	return 0 ; 
}
