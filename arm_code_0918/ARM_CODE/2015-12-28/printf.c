

int main(void)
{
	__asm__ __volatile__ (

	//数据段
	".data        \n"
	"str:		  \n"
	"	.string	\"hello world \\n\" \n"
	"	.align	5 \n"

	".text		  \n"
	"stmfd sp! , {r4-r12 , lr}   \n"
	//格式字符串
	"ldr	r0 , =str	\n"

	"bl		printf      \n"
	"ldmfd sp! , {r4-r12 , lr}   \n"
	);

	return 0 ; 
}
