

#include "res.h"
#include "Uart.h"
#include "rtc.h"
#include "serial.h"

print_t  printf = (void *)0x43E11A2C; 

int _start(void)
{

    printf("hello world \n");

#if 0
    //第一个串口
    s5p_serial0_device.init();
    
    delay(1000*1000);

    char buffer[100] = "hello world\n" ; 

    s5p_serial0_device.puts(buffer);

#endif
#if 0
    char ch ; 
    
    Init_Uart();

    uprintf("********************************\n");
    uprintf("*         Made in China        *\n");
    uprintf("*         Oh My God !!!        *\n");
    uprintf("*                 2015.12.31   *\n");
    uprintf("********************************\n");

    uprintf("See %d %d %d you Next %s \n" ,10,20,30, "year");

    char command[200] = {0};
    //自动计数功能
    char *CMD[] = {     //sizeof(CMD)/sizeof(CMD[0])
    "led_on",
    "led_off",
    "buzzer_on",
    "buzzer_off",
 //   "led_flash",
    "reset",
    "exit",
    };

    uprintf("Command Line : %d \n" , ARRAY_SIZE(CMD));
    int i ; 
    //configure  hardware
    led_configure();
    buzzer_configure();

    while(1)
    {
        uprintf("[root@cjy ]# ");
        gets(command);

        for(i = 0 ; i < ARRAY_SIZE(CMD) ; i++)       
        {
            if(strcmp(command , CMD[i]) == 0)       
                break;
        }

        switch(i)
        {
            case 0 :  led_all_on();  break;
            case 1 :  led_all_off(); break;
            case 2 :  buzzer_on();   break;       
            case 3 :  buzzer_off();  break;
//            case 4 :  led_flash();   break;
            case 4 :  SWRESET = 0x1 ;break;
            case 5 :  return 0 ; 
            defaule : uprintf("Command Not Found!\n"); break;
        }

        uprintf("%s\n",command );
    }
#endif

	return 0 ; 
}


