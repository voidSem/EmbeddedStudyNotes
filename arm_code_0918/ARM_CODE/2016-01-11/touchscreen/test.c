

#include "res.h"
#include "Uart.h"
#include "rtc.h"
#include "serial.h"
#include "i2c.h"
#include "pwm.h"

static print_t  printf = (void *)0x43E11A2C; 
static int count = 0 ; 
static unsigned int Code = 0 ; 
char buffer[32] = {0};

void Reset(void);
void Undef(void);
void Svc(unsigned int SystemCallNum);
void PrefetchAbt(void);
void DataAbt(void);
void Irq(void);
void Fiq(void);

int _start(void)
{

    //2.copy vector
    vector_copy();

    *(unsigned int *)0x62000000 = (unsigned int)Reset;
    *(unsigned int *)0x62000004 = (unsigned int)Undef;
    *(unsigned int *)0x62000008 = (unsigned int)Svc;
    *(unsigned int *)0x6200000C = (unsigned int)PrefetchAbt;
    *(unsigned int *)0x62000010 = (unsigned int)DataAbt;
    *(unsigned int *)0x62000014 = (unsigned int)Irq;
    *(unsigned int *)0x62000018 = (unsigned int)Fiq;

    //IO 初始化
    GPIO_Init();

    //定时器初始化
    timer_init();

    //1.enable mmu
    enable_mmu();
    
    //3.clear cpsr I bit
    enable_cpsr_irq();

	//unsigned int irqno = 64 ; 
	//
	//0xF   0b1111    cpu0 - cpu3   interrupt ID
	//enable_irq(0xF ,0x1 , irqno);
	
	//1M   100/1M S ==> 0.1ms

	Init_Timer(3,100);

	//Enable_Timer(0);
	//Disable_Timer(0);
	

	//enable timer
	//	TCON |= 1 ; 
	
	unsigned int tiggermode = 0x2 ; 
	enable_external_irq(tiggermode , 14);

    
    I2C_InitIp(I2C1 , 200000 , 10000);
//    I2C_Init_TouchScreen();


#if 0
	//EXT
	//	Sets signaling method 
	//	0x0 = Low level 
	//	0x1 = High level 
	//	0x2 = Triggers Falling edge 
	//	0x3 = Triggers Rising edge 
	//	0x4 = Triggers Both edge 
	//	0x5 to 0x7 = Reserved 
	//	总共有外部中断32    0->31

	unsigned int tiggermode = 0x4 ; 
	unsigned int eirqno = 26 ; 

	enable_external_irq(tiggermode , 26);
#endif

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
	static int i = 0x1 ; 
	//中断处理函数
	//不可以输出  因为输入输出函数都有效率问题  调试的时候可以打印
	//不可以阻塞，不可以睡眠
	//不可以操作浮点类型的算法
#if 1
	if(Check_Timer(3))
	{
		count++ ; 
        if(count >= 500)
        {
            Disable_Timer(3);   
            count = 0 ; 
            buzzer_off();
        }
	}
#endif
//	#define  GPX1DAT    0x11000C24
	if(clear_external_irq(14))
	{
        count = 0 ; 
        Enable_Timer(3);   
        buzzer_on();

        //通过主机I2C去读取从机的触摸数据
       
        I2C_Recv(I2C1 , 0x38<<1,buffer , 32 );
      //  Read_I2C_Data(0x38 , buffer , 32);
     
        printf("touch : %2d *****\n" , buffer[2] & 0xF);
        for(i = 0 ; i < (buffer[2]&0xF) ; i++)
        {
        printf("x: %5d  Y: %5d  \n", 
        ((buffer[i*6+3]&0xF)<<8) | (buffer[i*6+3+1]&0xFF),
        ((buffer[i*6+3+2]&0xF)<<8)|(buffer[i*6+3+3]&0xFF));
        }
        switch(buffer[1])
        {
        case 0x10:  printf("Move UP   \n"); break;       
        case 0x14:  printf("Move LEFT \n"); break;       
        case 0x18:  printf("Move DOWN \n"); break;       
        case 0x1C:  printf("Move RIGHT\n"); break;       
        case 0x48:  printf("Zoom in   \n"); break;       
        case 0x49:  printf("Zoom out  \n"); break;       
        case 0x00:  printf("no Gesture\n"); break;
        default :  printf("unknow ... \n");
        }

        printf("\n");
	}


}
void Fiq(void)
{
    printf("this is Fiq... \n");   
}

