
#include <stdio.h>

int main(void)
{
	
	unsigned int a = 0xabefc276 ;
	//0xabef0076   &  0xffff00ff   (~0x0000ff00)  ~(0xff<<8) 
//	a = a & (~(0xff << 8)) ; 
//	a &= ~(0xff << 8) ; //清位
//	a &= ~((1 << 6) | (1 << 13) | (1 << 26)) ; 

//	a &= ~(0xf | (0xf << 8)|(0xf << 20)|(0xf<<28));
	
	//清
//	a &= ~(0xff<<24) ;
	//置位
//	a |= 0x22 << 24; 

	//取反
	a ^= 0xff ; 
	

	printf("a : %p \n" , a);

	return 0 ; 
}
