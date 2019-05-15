
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
    char *p = "hello world and hello kitty ... \n" ; 

    write(fd , p , strlen(p));

    lseek(fd , 11 , SEEK_SET);
    
    p = "hahaha" ; 
    write(fd , p ,strlen(p));

    lseek(fd , 0 , SEEK_SET);

    read(fd , buffer , 30);
    printf("buffer : %s \n" , buffer);

    close(fd);

    return 0 ;
}
