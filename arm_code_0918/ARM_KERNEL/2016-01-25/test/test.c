
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>

int main(int argc  , char *argv[])
{

    int fd  = open("/dev/backlight-1wire" , O_WRONLY);
    if(fd < 0)
    {
        perror("Open device fail");
        return -1 ; 
    }

    unsigned int val ; 
        
    //val = atoi(argv[1]);
    
    write(fd , argv[1] , 3);

    close(fd);

    return 0 ;
}
