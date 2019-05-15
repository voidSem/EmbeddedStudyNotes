
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "../myioctl.h"


int main(void)
{

    int sockfd ; 

    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if(sockfd < 0)
    {
        perror("get sockfd fail");
        return -1 ; 
    }

    //
    struct sockaddr_in  addr ; 
    addr.sin_family = AF_INET ;
    addr.sin_port = htons(10086);
    addr.sin_addr.s_addr = inet_addr("192.168.88.254");

    int ret ; 
    ret = connect(sockfd , (struct sockaddr *)&(addr) ,  sizeof(struct sockaddr_in));
    if(ret < 0)
    {
        perror("connect fail");
        return -2 ; 
    }

    printf("connect success .. \n");
    unsigned int val ; 
    //打开设置节点
    int fd ; 
    fd = open("../KAIFA",O_RDWR);
    if(fd < 0)
    {
        perror("open device fail");
        return -3 ; 
    }
    

    while(1)
    {
        if(read(sockfd , &val , 4) <= 0)
            break;
        if(val == 0)
        {
            ioctl(fd , BUZZER_OFF);
        }else
        {
            ioctl(fd , BUZZER_ON);
        }

    }
    close(fd);

    return 0 ; 
}


