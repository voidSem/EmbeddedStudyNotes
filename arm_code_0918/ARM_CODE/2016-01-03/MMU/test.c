

#include "res.h"
#include "Uart.h"
#include "rtc.h"
#include "serial.h"

print_t  printf = (void *)0x43E11A2C; 


int _start(void)
{
    *(unsigned int *)0x5FFF0000 = 0x87654321 ; 

    enable_mmu();

    printf("value is : %p \n" , *(unsigned int *)0xFFFF0000);

	return 0 ; 
}


