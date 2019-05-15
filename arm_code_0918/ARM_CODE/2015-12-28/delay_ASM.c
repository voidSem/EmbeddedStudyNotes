
#include <stdio.h>
#include <sys/time.h>
#include <time.h>


int main(void)
{
	struct timeval  tv ; 
	struct timezone tz ; 

	gettimeofday(&tv , &tz);
	printf("before . sec:%u  usec:%u \n" ,tv.tv_sec , tv.tv_usec );
//xxx {S} <c><q>
#if 0
//	unsigned int val = 0xfffffff ; 
	__asm__ __volatile__ (
//	"mov	r0 , %0   \n"
	"ldr	r0 , =0xfffffff   \n"

	"again:           \n"

	"sub	r0 , r0 , #1   \n"
	"cmp	r0 , #0   \n"
	"bne	again     \n"
	);
#endif

#if 1
	__asm__ __volatile__ (
	"ldr	r0 , =0xfffffff   \n"

	"again:           \n"

	"subs	r0 , r0 , #1   \n"
	"bne	again     \n"
	);
#endif

	gettimeofday(&tv , &tz);
	printf("after . sec:%u  usec:%u \n" ,tv.tv_sec , tv.tv_usec );

	return 0 ;
}
