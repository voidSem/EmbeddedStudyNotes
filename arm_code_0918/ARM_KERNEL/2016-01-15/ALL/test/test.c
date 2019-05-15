
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include "../myioctl.h"


int main(int argc , char *argv[])
{
    if(argc != 2)
        return -1 ; 
    
    int fd ; 
    
    fd = open("../KAIFA" , O_RDWR);
    if(fd < 0)
    {
        perror("Open device fail");
        return -1 ; 
    }

    if(strcmp(argv[1] , "on") == 0)
    {
        ioctl(fd , BUZZER_ON);
    }

    if(strcmp(argv[1],"off") == 0)
    {
        ioctl(fd , BUZZER_OFF);
    }

    close(fd);

    return 0 ;
}


