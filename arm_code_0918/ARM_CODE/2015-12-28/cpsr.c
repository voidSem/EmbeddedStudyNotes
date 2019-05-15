
#include <stdio.h>

int main(void)
{
	//cpsr  状态寄存器  spsr  ==>   apsr
	//N     Z    C     V
	//负数 零   进位  溢出  
	//
	//cpsr  0 - 4   模式位  9种模式
	//
	//读取cpsr值  
	//mov  register  status
	//
	//MRS{<c>}{<q>}   <Rd>, <spec_reg>
	//
	//<spec_reg> Is one of:
	//          APSR
	//          CPSR.

	//写寄存器
	//mov	status   register
	//
	//MSR{<c>}{<q>}   <spec_reg>, <Rn>
	//
	//<spec_reg> Is one of:
	//          APSR
	//          CPSR.

	unsigned int cpsr ; 

	__asm__ __volatile__ (
	
	"mrs	r0 , cpsr   \n"
	"mov	%0 , r0     \n"
	:"=r"(cpsr)
	::"r0"
	);

	switch(cpsr&0x1F)
	{
	case  0x10 : printf(" User \n"); break;
	case  0x11 : printf(" FIQ  \n"); break;
	case  0x12 : printf(" IRQ  \n"); break;
	case  0x13 : printf(" SVC  \n"); break;
	case  0x16 : printf(" MONI \n"); break;
	case  0x17 : printf(" ABT  \n"); break;
	case  0x1A : printf(" HYP  \n"); break;
	case  0x1B : printf(" UNDEF\n"); break;
	case  0x1F : printf(" SYS  \n"); break;
	}

	unsigned int cpsr1 , cpsr2 ; 

	__asm__ __volatile__ (
	"mov	r0 , #1   \n"
	"mov	r1 , #2   \n"

	"mrs	r2 , cpsr \n"
	"cmp	r0 , r1   \n"
	"mrs	r3 , cpsr \n"

	"mov	%0 , r2   \n"
	"mov	%1 , r3   \n"
	:"=r"(cpsr1),"=r"(cpsr2)
	::"r0","r1","r2","r3"
	);
	printf("cpsr1 : %p \n" , cpsr1);
	printf("cpsr2 : %p \n" , cpsr2);

	return 0 ;
}
