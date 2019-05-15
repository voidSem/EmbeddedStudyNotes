
#include "res.h"

int led_configure(void)
{
	//set GPM4CON 16bits as output
//	GPM4CON &= ~0xffff ;
//	GPM4CON |= 0x1111 ; 
//	led_all_off();
	int i ; 
	for(i = 0 ; i < 4 ; i++)
	{
		gpio_direction_output((void *)BGPM4CON ,  i , GPIO_HIGHT );
	}
	return 0 ; 
}
int led_all_on(void)
{
//	GPM4DAT &= ~0xf ; 
	int i ; 
	for(i = 0 ; i < 4 ; i++)
	{
		gpio_set_value((void *)BGPM4CON ,  i , GPIO_LOW);
	}
	return 0 ; 
}
int led_all_off(void)
{
	//GPM4DAT |= 0xf ; 
	int i ; 
	for(i = 0 ; i < 4 ; i++)
	{
		gpio_set_value((void *)BGPM4CON ,  i , GPIO_HIGHT);
	}
	return 0 ; 
}
//0-3
int led_which_on(int which)
{
//	GPM4DAT &= ~(0x1 << which);
	gpio_set_value((void *)BGPM4CON , which , GPIO_LOW);
	return 0 ; 
}
int buzzer_configure(void)
{
//	GPD0CON &= ~0xf ;
//	GPD0CON |= 0x1 ; 
//	buzzer_off();
	gpio_direction_output((void *)BGPD0CON , 0 , GPIO_LOW);
	return 0 ; 
}
int buzzer_on(void)
{
//	GPD0DAT |= 1 ; 	
	gpio_set_value((void*)BGPD0CON , 0 , GPIO_HIGHT);
	return 0 ; 
}
int buzzer_off(void)
{
//	GPD0DAT &= ~0x1 ;
	gpio_set_value((void*)BGPD0CON , 0 , GPIO_LOW);
	return 0 ; 
}
int key_configure(void)
{
	int i ; 
	for(i = 2 ; i < 6 ;i++)
	{
		gpio_direction_input((void *)BGPX3CON , i);
	}
	//GPX3CON &= ~(0xffff << 8) ;
	return 0 ; 
}

int get_key(void)
{
	unsigned int val ; 
	int i ; 
	
	while(1)
	{
//		val = (GPX3DAT >> 2) & 0xf ;
		for(i = 2 ; i < 6 ; i++)
		{
			if(!gpio_get_value((void *)BGPX3CON , i))
				return  i-2 ; 
		}

	}
#if 0
	int i ; 
	for(i = 0 ; i < 4 ; i++)
	{
		if(!(val & (0x1 << i)))
			return i ; 
	}
#endif
	return 0 ; 
}


int delay(unsigned int val)
{
	while(val--) ;
}


int led_flash(void)
{
	led_configure();
	buzzer_configure();

	while(1)
	{
		led_all_on();
		buzzer_on();
		delay(0xfffff);
		led_all_off();
		buzzer_off();
		delay(0xfffff);
	}
}

int led_run(void)
{

	led_configure();
	buzzer_configure();

	int i ;
	for(i = 0 ;  ; i++ )
	{
		if(i == 4)  
			i = 0 ; 
		led_which_on(i);
	//	buzzer_on();
		delay(0xfffff);
		led_all_off();
	}
}
