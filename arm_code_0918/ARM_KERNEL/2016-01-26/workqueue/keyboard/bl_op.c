
#include "bl_op.h"

static int fd ; 
static int value = 0; 
static char buffer[4] = {0,0,0,0};

void Init_Backlight(void)
{

    fd  = open("/dev/backlight-1wire" , O_WRONLY);
    if(fd < 0)
    {
        perror("Open device fail");
        return ; 
    }
}

void Exit_Backlight(void)
{
    close(fd);
}

int Add_Light(void)
{
    value += 10 ;
    if(value >= 127)
        value = 127 ;
    sprintf(buffer , "%u" , value);
    write(fd , buffer , 4);    //atoi
}

int Sub_Light(void)
{
    value -= 10 ;
    if(value <= 0)
        value =0 ;
    sprintf(buffer , "%u" , value);
    write(fd , buffer , 4);    //atoi
}

int Max_Light(void)
{
    value = 127 ;
    sprintf(buffer , "%u" , value);
    write(fd , buffer , 4);    //atoi
}

int Min_Light(void)
{
    value = 0 ;
    sprintf(buffer , "%u" , value);
    write(fd , buffer , 4);    //atoi
}
