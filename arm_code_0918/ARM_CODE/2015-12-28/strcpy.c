
#include <stdio.h>


int main(void)
{
	char *p = "hello world" ; 
	char buffer[100]  ; 
	
	__asm__ __volatile__ (

	//加载
	"mov	r0 , %0   \n"
	"mov	r1 , %1   \n"


	"again: 			   \n"
	"ldrb	r2 , [r0]      \n"
	"strb	r2 , [r1]      \n"
	"cmp	r2 , #0        \n"

	"addne	r0 , r0 , #1   \n"
	"addne	r1 , r1 , #1   \n"
	"bne	again		   \n"


	::"r"(p),"r"(buffer)
	:"r0","r1","r2"
	);

	printf("buffer: %s \n" , buffer);

	return 0 ; 
}
