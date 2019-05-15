

#include "res.h"

int _start(void)
{
	led_configure();
	buzzer_configure();
	key_configure();

	int num ;
	while(1)
	{
		num  = get_key();
		led_which_on(num);
		buzzer_on();
		delay(0xfffff);
		led_all_off();
		buzzer_off();
		if(num == 3)
			break;
	}

	return 0 ; 
}


