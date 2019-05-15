

#include "res.h"
#include "Uart.h"
#include "rtc.h"
#include "serial.h"

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

    //buzzer_configure();
    //触摸屏的从机地址    0x38

        
   //时钟 心跳 
   //设置  100M / 512  ==> 200K  200K / 2 => 100K
   I2CCONn |= 1 << 6 ; 

   I2CCONn &= ~0xF ; 
   I2CCONn |= 0x1  ; 

   //GPIO模式选择
   #define  GPD1CON    *(U32 *)0x114000C0
   GPD1CON &= ~(0xFF << 8) ; 
   GPD1CON |= 0x22 << 8 ; 

   //逻辑
   //enable ACK       
   I2CCONn |= 1 << 7 ; 

   //enable irq
   I2CCONn |= 1 << 5 ; 

   //set Master Recive
   I2CSTATn &= ~(0x3 << 6) ; 
   I2CSTATn |= 0x2 << 6 ; 
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
	enable_external_irq(tiggermode , 27);
	enable_external_irq(tiggermode , 28);
	enable_external_irq(tiggermode , 29);
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

static int Wait_for_irq(void)
{
    int i;   
    for(i = 0 ; i < 0x3FFF ; i++)
    {
        if(I2CCONn & (1 << 4))
            return 1;   //有数据接收
    }
    return 0 ; 
}

static int Wait_for_ACK(void)
{
    int i ; 
    for(i = 0 ; i < 0x3FF ; i++)
    {
        if(!(I2CSTATn & 1))
            return 1 ; //有ACK
    }
    return 0 ; 
}

static int clear_i2c_irq(void)
{    
    I2CCONn &= ~(1 << 4);
}

static void Read_I2C_Data(unsigned int SlaveAddr , char *buffer , int len)
{
    //Master Recv
    I2CSTATn &= ~(0x3 << 6) ; 
    I2CSTATn |= 0x2 << 6 ; 
    
    //slave address
    I2CDSn = (SlaveAddr << 1) | 0x1 ; 

    //send start bit
    I2CSTATn = 0xB0 ;

    clear_i2c_irq();

    if(!Wait_for_irq())
    {
        printf("first wait irq fail \n");
        goto exit;
    }

    //wait for ACK
    if(!Wait_for_ACK())
    {
        printf("wait for ack fail \n");
        goto exit ; 
    }

    int i ; 
    for(i = 0 ; i < len ; i++)
    {
        clear_i2c_irq();
        if(!Wait_for_irq())
        {
            printf("second wait irq fail \n");
            goto exit ; 
        }
        buffer[i] = I2CDSn ; 
    }

exit:
    I2CSTATn = 0x90 ; 

    clear_i2c_irq();

    delay(1000);
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
//	if(TINT_CSTAT & (1 << 5))
	{
		//printf("timer coming ... \n");
		count++ ; 
//		TINT_CSTAT |= 1 << 5 ; 
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
       
        Read_I2C_Data(0x38 , buffer , 32);
     
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

#if 0
	//clear
	CLRINTADC = 0 ; 
	printf("DO : %p \n" , ADCDAT & 0xfff);
	ADCCON |=  1 ;


	//clear watch dog irq
	printf("wang wang wang ... \n");
	WTCLRINT = 0 ; 


	char ch ='A'; 
	//fifo node
	if(UINTPn & 1 )
	{
		//ch = URXHn ; 
		printf("hello uart ... %c %d \n" , ch , UFSTATn&0xFF);
		UINTPn |= 1 ; 
	}

	//no fifo mode
	if(UINTPn & 1 )
	{
		ch = URXHn ; 
		printf("hello uart ... %c \n" , ch);
		UINTPn |= 1 ; 
	}

	if(INTP & 1)
	{
		INTP |= 1 ; 
		printf("time out ... \n");
	}

	int eirqno = 26 ; 
	
	if(clear_external_irq(26))
	{
		printf("key 0 some one press me ...\n");
	}

	if(clear_external_irq(27))
	{
		printf("key 1 some one press me ...\n");
	}

	if(clear_external_irq(28))
	{
		printf("key 2 some one press me ...\n");
	}

	if(clear_external_irq(29))
	{
		printf("key 3 some one press me ...\n");
	}

	if(EXT_INT43_PEND & (1 << 2))
	{
		//清中断
		EXT_INT43_PEND = EXT_INT43_PEND ; 
		printf("some one press me ... \n");
	}
#endif

}
void Fiq(void)
{
    printf("this is Fiq... \n");   
}

