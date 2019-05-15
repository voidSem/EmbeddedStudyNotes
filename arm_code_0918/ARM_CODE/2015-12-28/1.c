
#include <stdio.h>

int main(void)
{
	int a , b ; 
	//字符串本身具有自动粘贴功能
//	printf("hello" "  world" "  " "\n");

	__asm__ __volatile__ (
	//循环右移偶数位之后八位可以存得下的数字不会报错
	//
	"mov	r0 , #256   \n"
	"mov	r1 , #44   \n"

	"mov	%0 , r0    \n"
	"mov	%1 , r1    \n"

	:"=r"(a),"=r"(b)
	::"r0","r1"  //使用过哪些寄存器
	);

	printf("a : %d  b : %d \n" , a , b);

	return 0 ; 
}
