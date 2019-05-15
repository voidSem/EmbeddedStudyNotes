
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

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
    
    char buffer[100] = {0};

    char *p = "hello world" ; 

    ret = write(fd ,  p ,strlen(p));
    if(ret != strlen(p))
    {
        perror("write date fail");
        printf("ret : %d  len:%d \n" , ret , strlen(p));
        return -1 ; 
    }

    close(fd);

    return 0 ;
}
