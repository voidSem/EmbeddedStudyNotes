
#include <stdio.h>

__asm__ (
".global	HAHA       \n"
"HAHA:				   \n"

"mov	pc , lr        \n"

);

__asm__ (
".global	add_func   \n"
"add_func:			   \n"
"stmfd sp! , {r4-r12 , lr}   \n"

"add	r2 , r0 , r1   \n"

"mov	lr , pc        \n"
"b	HAHA               \n"

"ldmfd sp! , {r4-r12 , lr}   \n"
//恢复现场
"mov	pc , lr        \n"
);

int main(void)
{
	//汇编规定    APCS   r0 r1 r2 r3  不受保护区域
	//
	int sum ; 

	__asm__ __volatile__ (  //本身有压栈

	"mov	r0 , #33    \n"
	"mov	r1 , #44    \n"
	//保护现场
	"mov	lr , pc     \n"
//	"nop	\n"    //不可以插入指令
	"b	add_func		\n"
	
	"mov	%0 , r2     \n"
	//"bl	add_func        \n"  //自动保护现场
	:"=r"(sum)
	::"r0","r1","r2"
	);

	printf("sum : %d  \n" , sum);

	return 10 ;
}

