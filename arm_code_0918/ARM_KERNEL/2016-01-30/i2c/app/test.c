
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <linux/input.h>

int main(void)
{
    int fd ; 


    fd = open("/dev/event0" , O_RDWR);
    if(fd < 0)
    {
        perror("open device fail");
        return -1 ; 
    }
    struct input_event event[5] ;    
    int ret ; 

    while(1)
    {
        ret = read(fd , event ,  sizeof(struct input_event)*5);
        if(ret != 80)
            continue ; 
        //printf("type : %x  %x  %x  %x  %x \n" , event[0].code , event[1].code , event[2].code , event[3].code , event[4].code  );
//       printf("start ********************************\n");
//        printf("type:%d  code:%x  value:%d  \n" , event[0].type ,event[0].code , event[0].value );
//        printf("type:%d  code:%x  value:%d  \n" , event[1].type ,event[1].code , event[1].value );
//       printf("type:%d  code:%x  value:%d  \n" , event[2].type ,event[2].code , event[2].value );
//        printf("type:%d  code:%x  value:%d  \n" , event[3].type ,event[3].code , event[3].value );
//        printf("ret : %d size:%d \n", ret , sizeof(struct input_event));
        printf("x : %d  y: %d %d %d\n" , event[0].value , event[1].value,event[2].value , event[3].value);
    }

    close(fd);


    return 0 ;
}
