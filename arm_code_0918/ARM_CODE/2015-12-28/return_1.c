
int main(void)
{
	__asm__ __volatile__ (
	//返回值默认就是r0的值
	"mov	r0 , #54  \n"
	
	"mov	pc , lr   \n"
	);


	return 300; 
}
