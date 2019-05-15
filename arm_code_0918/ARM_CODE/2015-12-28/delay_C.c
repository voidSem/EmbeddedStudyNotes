
#include <stdio.h>
#include <sys/time.h>
#include <time.h>


int main(void)
{
	struct timeval  tv ; 
	struct timezone tz ; 

	gettimeofday(&tv , &tz);
	printf("before . sec:%u  usec:%u \n" ,tv.tv_sec , tv.tv_usec );

	unsigned int val = 0xfffffff ; 
	//1700ms
	//
	while(val--) ; 


	gettimeofday(&tv , &tz);
	printf("after . sec:%u  usec:%u \n" ,tv.tv_sec , tv.tv_usec );

	return 0 ;
}
