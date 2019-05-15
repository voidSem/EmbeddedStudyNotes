
#include "res.h"


void Init_Timer(unsigned int Timer,unsigned int usec)
{//范围Timer  0-4

	Timer %= 5 ; 
	
	enable_irq(0xF , 0x1 , 69+Timer);

	if(Timer <= 1)
	{
	//timer  PWM   first  prescale
	TCFG0 = 99 ;   //100M / (99+1)  ==> 1M      1Hz => 1us
	//set second prescale
	TCFG1 = 0 ; 
	}else
	{
	//timer  PWM   first  prescale
	TCFG0 = 99 << 8 ;
	//set second prescale
	TCFG1 = 0 ; 
	}

	//auto reload
	TCON |= (1 << (3 +(!!Timer)*(4+Timer*4))) ; 

	//invert on
	TCON |= 1 << (2+(!!Timer)*(4+Timer*4)) ; 

	//set timer counter Buffer
	//TCNTB0 = 100 ;  //0.1ms
	*(U32 *)(0x139D000C + Timer * 0xC) = usec ; 
	
	//set timer compare buffer
	//TCMPB0 = 1000 ; 
	
	//note:  must update manaul
	TCON |= 1 << (1 + (!!Timer)*(4+Timer*4)) ; 

	//must clear update
	TCON &= ~(1 << (1+(!!Timer)*(4+Timer*4))) ; 

	//decide enable interrupt
	TINT_CSTAT |= 1 << Timer ; 

}
void Enable_Timer(unsigned int Timer)
{	
	TCON |= 1 << ((!!Timer)*(4+Timer*4)) ; 
}
void Disable_Timer(unsigned int Timer)
{
	TCON &= ~(1 << ((!!Timer)*(4+Timer*4))) ; 
}

int Check_Timer(unsigned int Timer)
{
	if(TINT_CSTAT & (1 << (Timer+5)))
	{
		TINT_CSTAT |= 1 << (Timer + 5) ; 
		return 1 ; 
	}
	return 0 ; 
}

