
#include <stdio.h>

int main(void)
{
	int sum ; 	

	__asm__ __volatile__ (

	//mov     #
	//ldr	  =
	
	"ldr	r0 , =0xfffff   \n"
	"ldr	r1 , =0xfffff   \n"

	// { } 可选
	//ADD{S}{<c>}{<q>}  {<Rd>,} <Rn>, #<const>
	//ADD{S}{<c>}{<q>}  {<Rd>,} <Rn>, <Rm> {, <shift>}
	//   <Rd> = Rn + Rm ; 
//	"add	r2 , r0 , r1    \n"
//	"mov	%0 , r2         \n"
	
	//  r0 = r0 + r1 ;   r0+=r1 ; 
//	"add	r0 , r1 ,lsr	#19   \n"  //带shift add
	"add	r0 , r0 , #(1 << 20)  \n"

	"mov	%0 , r0         \n"
	
	//"add	r0 , #33 , #44  \n"  Fail

	:"=r"(sum)
	::"r0","r1","r2"
	);
	
	printf("sum : %p \n" , sum);

	return 0 ; 
}
