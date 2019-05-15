
//SFR
//--> led
#define     GPM4CON    0x110002E0
#define     GPM4DAT	   0x110002E4
//--> buzzer
#define     GPD0CON    0x114000A0
#define     GPD0DAT    0x114000A4
//--> key
#define     GPX3CON    0x11000C60
#define     GPX3DAT    0x11000C64

int (*printf)(const char *fmt , ...) = (void * )0xC3E11ad0;

int a = 0 ; 
int b ;

int c ; 
int c ; 
int c ; 
int c ; 
int c ; 
int c = 1 ; 

int _start(void)
{

	printf("hello world a:%d b:%d c:%d\n" , a , b , c );

	return 0 ; 
}

