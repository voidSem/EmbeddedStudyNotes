

#include "res.h"
#include "Uart.h"
#include "rtc.h"
#include "serial.h"

print_t  printf = (void *)0x43E11A2C; 

void Reset(void);
void Undef(void);
void Svc(unsigned int SystemCallNum);
void PrefetchAbt(void);
void DataAbt(void);
void Irq(void);
void Fiq(void);
void vector_copy(void);

int _start(void)
{
    vector_copy();

    *(unsigned int *)0x62000000 = (unsigned int)Reset;
    *(unsigned int *)0x62000004 = (unsigned int)Undef;
    *(unsigned int *)0x62000008 = (unsigned int)Svc;
    *(unsigned int *)0x6200000C = (unsigned int)PrefetchAbt;
    *(unsigned int *)0x62000010 = (unsigned int)DataAbt;
    *(unsigned int *)0x62000014 = (unsigned int)Irq;
    *(unsigned int *)0x62000018 = (unsigned int)Fiq;

    enable_mmu();
    


    printf("before exception \n");



    __asm__ __volatile__ (
    //未定义指令  超级用户调用都在编译产生   
    
    ".word  0x77777777   \n"  //未定义指令  异常
    ".word  0x77777778   \n"
    ".word  0x77777779   \n"

    "SVC    #55          \n"  //超级用户调用
    "swi    #66          \n"

    );

    //数据异常   在执行的时候产生的
    //中断异常   在执行的时候产生的
    //
    *(unsigned int *)0x66111111 = 100 ; 
    
    *(unsigned int *)0x90000000 = 200 ; 


    printf("after exception \n");

	return 0 ; 
}


void Reset(void)
{
    printf("this is reset ... \n");   
}
void Undef(void)
{
    printf("this is Undef... \n");   
}
void Svc(unsigned int SystemCallNum)
{
    printf("this is Svc... %d \n" , SystemCallNum);   
}
void PrefetchAbt(void)
{
    printf("this is PrefetchAbt... \n");   
}
void DataAbt(void)
{
    printf("this is DataAbt... \n");   
}
void Irq(void)
{
    printf("this is Irq... \n");   
}
void Fiq(void)
{
    printf("this is Fiq... \n");   
}

void vector_copy(void)
{
    extern void start(void);
    extern void end(void);
    //加尾部
    unsigned int offset = (unsigned int )end - (unsigned int )start + 4; 

    printf("offset : %d \n" , offset);

    memcpy((char *)0x5FFF0000 , (char *)start , offset + 200);

}
