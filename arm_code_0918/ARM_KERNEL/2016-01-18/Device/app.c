
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "deviceioctl.h"
#include <sys/ioctl.h>

int main(int argc , char *argv[])
{
    int fd ;       

    fd = open("DEVICE" , O_RDWR);
    if(fd < 0)
    {
        perror("open DEVICE fail ");
        return -1 ;
    }

    int ret ;   
    
    char buffer[10] = "hello";

    ret =  read(fd , buffer , 10);
    printf("ret : %d  buffer:%s \n" , ret , buffer);

    //************************************
    
    
    if(strcmp(argv[1] , "led_on") == 0)
    {
        ioctl(fd , LED_ON);
    }

    if(strcmp(argv[1] , "led_off") == 0)
    {
        ioctl(fd , LED_OFF);
    }

    if(strcmp(argv[1] , "buzzer_on") == 0)
    {
        ioctl(fd , BUZZER_ON);
    }

    if(strcmp(argv[1] , "buzzer_off") == 0)
    {
        ioctl(fd , BUZZER_OFF);
    }


    close(fd);

    return 0 ;
}
