

#include "res.h"
#include "rtc.h"
//RTC

print_t  printf = (void *)0xC3E11ad0 ; 

int _start(void)
{
	//1970   2000

//	int tm_sec;
//	int tm_min;
//	int tm_hour;
//	int tm_mday;
//	int tm_mon;
//	int tm_year;
//	int tm_wday;
//	int tm_yday;
//	int tm_isdst;
#if 0
    struct  rtc_time  Time = {
    0,1,18,30,12,2015,4
    }; 
    

    rtc_set(&Time);
#endif

#if 1
    struct rtc_time   Time ; 

    rtc_get(&Time);

    printf("year:%4d month:%2d day:%2d dayweek:%2d hour:%2d min:%2d sec:%2d \n" , Time.tm_year , Time.tm_mon , Time.tm_mday , Time.tm_wday , Time.tm_hour , Time.tm_min ,Time.tm_sec);
#endif 


#if 0
  
    int a = 10 ;
	int b = 4 ; 
    b = a / b ; 
    b = a % b ; 
	//开启写的模式
	RTCCON |= 1 ; 

	BCDSEC = 0 ; 
	BCDMIN = (5<<4 | 8) ; 
	BCDHOUR = (2 << 4 | 3) ; 

	BCDDAYWEEK = 7 ; 
	BCDDAY = 2 ; 

	BCDMON = 1 ; 

	BCDYEAR = (1 << 4 )| 6; 


	//关闭写的模式
	RTCCON &= ~0x1 ; 
#endif


#if 0
	printf("sec : %2d \n" , ((BCDSEC>>4)&0x7)*10 + (BCDSEC&0xf));
	printf("min:%2d \n" , ((BCDMIN>>4)&0x7)*10 + (BCDMIN&0xf));
	printf("hour:%2d\n" , ((BCDHOUR>>4)&0x3)*10 + (BCDHOUR&0xf));
	
	//*****************
	printf("day:%2d \n" , BCDDAY & 0x7);
	printf("dayweek:%2d\n" , ((BCDDAYWEEK>>4)&0x3)*10 + (BCDDAYWEEK&0xf));

	//*****************
	printf("month:%2d\n",((BCDMON>>4)&0x1)*10 + (BCDMON&0xf));
	printf("year:%4d\n" , ((BCDYEAR>>8)&0xF)*100+((BCDYEAR>>4)&0xF)*10 + (BCDYEAR&0xf));
#endif



	return 0 ; 
}


