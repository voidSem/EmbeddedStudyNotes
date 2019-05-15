
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd; 

    fd = open("/dev/mymisc" , O_RDONLY);
    if(fd < 0)
    {
        perror("open /dev/mymisc fail");       
        return -1 ;
    }

    sleep(10);


    close(fd);

    return 0 ;
}
