
#include <stdio.h>

int main(void)
{
	int a , b ; 
	int c ; 

	__asm__ __volatile__ (
	
	"mov	r0 , #44   \n"  //立即数
	"mov	r1 , #55   \n"
	"mov	r2 , #1024  \n"

	"mov	%0 , r2    \n"
	"mov	%1 , r0    \n"
	"mov	%2 , r1    \n"
	
	//从左到右，从上到下  从0开始往后递增
	:"=r"(c),"=r"(a),"=r"(b)    //输出  内嵌汇编输出给C
	:					//输入
	:"r0","r1"			//用过的寄存器
	);

	printf("a : %d b:%d  c: %d \n" , a , b , c);



	return 0 ;
}
