
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>
#include "bl_op.h"

#if 0
struct input_event {
	struct timeval time;
	__u16 type;
	__u16 code;
	__s32 value;
};
#endif
int main(void)
{
    struct input_event  event ; 


    int fd  = open("/dev/mymisc" , O_RDONLY);
    if(fd < 0)
    {
        perror("Open device fail");
        return -1 ; 
    }
    struct tm  *tt ; //localtime
    Init_Backlight();

    while(1)
    {
        read(fd , &event , sizeof(struct input_event));   
//        printf("time:%p \n" , &(event.time.tv_sec));
//        time_t  ta = event.time.tv_sec ; 
//        time_t  *tp = &ta ; 
//        tt = localtime(&(event.time.tv_sec));
//        printf("min:%d sec:%d type:%d code:%d value:%d \n" ,tt->tm_min , tt->tm_sec , event.type , event.code , event.value);
        
        if(event.value == 0)
        {
            switch(event.code)
            {
            case KEY_A : Sub_Light();       break;   
            case KEY_B : Add_Light();       break;
            case KEY_C : Min_Light();       break;
            case KEY_D : Max_Light();       break;
            }
        }
    }

    Exit_Backlight();
    return 0 ;
}
