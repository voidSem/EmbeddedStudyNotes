
#include <stdio.h>

int main(void)
{
	int a , b , c ; 
//	int buffer[10] ; 

	__asm__ __volatile__ (

	//r0-r12  sp  lr pc  cpsr  spsr
	
	"mov	r0 , #1   \n"
	"mov	r1 , #2   \n"
	"mov	r2 , #3   \n"

	//***********************
	//操作r0 r1 r2
	//压栈   先入后出    对数据保护
	//方法一      没有说明栈顶所在的位置  默认位置
//	"push	{r0}      \n"
//	"push	{r1}      \n"
//	"push	{r2}      \n"
	//怎么压怎么出 两种方式
	//方法二      没有说明栈顶所在的位置  默认位置
//	"push	{r0-r2}   \n"

	//方法三   ! 更新sp指针(会变)
	
//	"mov	fp , sp	        \n"
//	"mov	sp , %3         \n"
	"stmfd sp! , {r0-r2}	\n"
	
	//用
	"mov	r0 , #11  \n"
	"mov	r1 , #21  \n"
	"mov	r2 , #31  \n"

	//出栈  反着来
	//"pop	{r0-r2}   \n"
	//"pop 	{r2}      \n"
	//"pop 	{r1}      \n"
	//"pop 	{r0}      \n"
	"ldmfd sp! , {r0-r2}	\n"
//	"mov	sp , fp   \n"

	"mov	%0 , r0   \n"
	"mov	%1 , r1   \n"
	"mov	%2 , r2   \n"


	:"=r"(a),"=r"(b),"=r"(c)
	://"r"(buffer)
	:"r0","r1","r2"
	);

//	a = buffer[0];
//	b = buffer[1] ;
//	c = buffer[2] ;
#if 0
	int i ; 
	for(i = 0 ; i < 10 ; i++)
	{
		printf("buffer[%d]: %d \n" , i , buffer[i]);
	}
#endif

	printf("a : %d b : %d c : %d \n" , a , b , c );
	return 0 ; 
}
