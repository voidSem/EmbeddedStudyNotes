
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

    ret = read(fd , buffer , 11);
    if(ret < 0)
    {
        perror("read fail");
        printf("ret : %d  buffer:%s \n" , ret , buffer);
        return -1 ; 
    }

    printf("buffer : %s \n" , buffer);

    close(fd);

    return 0 ;
}
