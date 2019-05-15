
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

struct input_event event ;  //??

int main(void)
{
    int fd ; 

    fd = open("/dev/event3" , O_RDWR);
    if(fd < 0)
    {
        perror("Open event3 fail");
        return -1 ; 
    }

    int ret ; 
    
    while(1)
    {
        ret = read(fd , &event , sizeof(struct input_event));
        if(ret != sizeof(struct input_event))
            continue ; 
        printf("type:%d  code:%d  value:%d  \n" ,  event.type , event.code , event.value);
    }

    close(fd);

    return 0 ; 
}
