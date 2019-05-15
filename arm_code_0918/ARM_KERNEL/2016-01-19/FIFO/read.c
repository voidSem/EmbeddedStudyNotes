
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

struct student
{
    char name[13] ; 
    int id ; 
    float math ; 
};

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
    
#if 0
    char buffer[100] = {0};
    ret = read(fd , buffer , 11);
    if(ret < 0)
#endif
    struct student stu ; 
    ret = read(fd , &stu , sizeof(stu));
    if(ret < 0)
    {
        perror("read fail");
//printf("ret : %d  buffer:%s \n" , ret , buffer);
        return -1 ; 
    }

        printf("name:%s id:%d math:%f \n" , stu.name , stu.id , stu.math);
//    printf("buffer : %s \n" , buffer);

    close(fd);

    return 0 ;
}
