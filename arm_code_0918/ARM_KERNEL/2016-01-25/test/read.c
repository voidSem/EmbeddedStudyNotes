
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>

int main(int argc  , char *argv[])
{

    int fd  = open("/dev/touchscreen-1wire" , O_RDONLY);
    if(fd < 0)
    {
        perror("Open device fail");
        return -1 ; 
    }

    unsigned int val ; 
    while(1)
    {
        read(fd , &val , 4);   
        printf("val : %d \n" , val);
    }

    close(fd);

    return 0 ;
}
