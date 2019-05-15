
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd ; 

    fd = open("/dev/kitty" , O_RDWR);
    if(fd < 0)
    {
        perror("open device fail");
        return -1 ; 
    }

    sleep(30);


    close(fd);   

    return 0 ;
}
