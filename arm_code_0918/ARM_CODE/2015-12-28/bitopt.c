
#include <stdio.h>

int main(void)
{
	unsigned  int sum ; 

	//and 与   orr  或   bic  清位
	__asm__ __volatile__ (

	"ldr	r0 , =0xff00ff   \n"

	//置位
	"orr	r0 , r0 , #(0xf<<8)   \n"

	//清位
	"bic	r0 , r0 , #0xf   \n"

	//

	"mov	%0 , r0    \n"
	:"=r"(sum)
	::"r0"
	);

	printf("sum : %p \n" , sum);
	return 0 ; 
}
