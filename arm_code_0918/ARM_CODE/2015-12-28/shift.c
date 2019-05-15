
#include <stdio.h>

int main(void)
{
	
	unsigned int sum ; 
	//logical shift left
	//LSL{S}{<c>}{<q>}   {<Rd>,} <Rm>, #<imm5>
	//LSL{S}{<c>}{<q>}   {<Rd>,} <Rn>, <Rm>
	

	//lsl  lsr         logical shift right
	//(asl)(no exist)   asr 

	__asm__ __volatile__ (

//	"mov	r0 , #1   \n"
//	"lsL	r1 , r0 , #8   \n"  //立即数不可以超过32
//	"LsL	r0 , #1   \n"
//	"mov	r1 , #8   \n"
//	"ASL	r2 , r0 , r1   \n"
//	"mov	%0 , r2   \n"

	"ldr	r0 , =0xfffffff   \n" 
	//"lsr	r1 , r0 , #4       \n"
	"asr	r1 , r0 , #4       \n"

	"mov	%0 , r1   \n"

	:"=r"(sum)
	::"r0","r1"
	);

	printf("sum : %p \n" , sum);
	return 0 ; 
}
