
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/joystick.h>

#if 0
struct js_event {
	__u32 time;	/* event timestamp in milliseconds */
	__s16 value;	/* value */
	__u8 type;	/* event type */
	__u8 number;	/* axis/button number */
};
#endif
int main(void)
{
    int fd ; 

//    fd = open("/dev/input/js0" , O_RDONLY);  //PC
    fd = open("/dev/js0" , O_RDONLY);  //PC
    if(fd < 0)
    {
        perror("Open js fail");
        return -1 ;
    }

    struct  js_event  js ;    

    while(1)
    {
        read(fd , &js , sizeof(js));   
        
        printf("time:%15d value:%d type:%d number:%d \n" , 
            js.time , js.value , js.type , js.number);
    }

    return 0 ;
}

