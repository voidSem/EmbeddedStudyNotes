

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
	

	//Init_Timer(3,100);

	//Enable_Timer(0);
	//Disable_Timer(0);

	

    
    I2C_InitIp(I2C3 , 200000 , 10000);
//    I2C_Init_TouchScreen();

    unsigned char buffer[20] = {0};

    //默认为standby 模式   注意：只有在standby模式下才能修改非XYZ寄存器的值
    #define  SLAVEADDR   (0x4C<<1)

/*
 *test
 * */
    printf("hello\n");
    __udelay(1000*1000);
    __udelay(1000*1000);
    printf("goodbey\n");


    unsigned char ADDR[1] ; 
    unsigned char VAL[1] ;
#if 0
    //1.set test mode
    ADDR[0] =  0x07 ; 
    VAL[0] = 0x4 ; 

    I2C_SendForGeneral(I2C3 ,SLAVEADDR ,ADDR , 1 , VAL , 1 );   

    //set x as 0x3F
    ADDR[0] = 0x0 ;  //x
    VAL[0] = 0x3F ; 
    I2C_SendForGeneral(I2C3 ,SLAVEADDR ,ADDR , 1 , VAL , 1 );   



     
    //设置读取时不能偏移，设置时可以偏移 
    I2C_RecvForGeneral(I2C3 ,SLAVEADDR , ADDR , 1 , buffer, 10);
    //I2C_Recv(I2C3 , SLAVEADDR , buffer , 10);
    printf("mode:%p \n" ,buffer[7]);
    printf("x : %p  y:%p  z:%p \n" , buffer[0],buffer[1],buffer[2]);
    //stop  确定I2C跟MMA通信正常  需要测试
#endif

    //先开25号外部中断   raising??
	unsigned int tiggermode = 0x3 ; 
	enable_external_irq(tiggermode , 25);

    //1.set standby mode
    ADDR[0] =  0x07 ; 
    VAL[0] = 0x0 ; 
    I2C_SendForGeneral(I2C3 ,SLAVEADDR ,ADDR , 1 , VAL , 1 );   

    //2.set sample
    ADDR[0] = 0x08 ; 
    VAL[0] = 0x0 ;   //120sample per second
    I2C_SendForGeneral(I2C3 ,SLAVEADDR ,ADDR , 1 , VAL , 1 );   
    

    //开中断
    ADDR[0] = 0x06 ; 
    VAL[0] = 0x01 ;   //front and back irq
    I2C_SendForGeneral(I2C3 ,SLAVEADDR ,ADDR , 1 , VAL , 1 );   


    //3.set auto sleep and active mode
    ADDR[0] = 0x07 ; 
    VAL[0] = 0xd1 ;  //
    I2C_SendForGeneral(I2C3 ,SLAVEADDR ,ADDR , 1 , VAL , 1 );   

//    I2C_Recv(I2C3 , SLAVEADDR , buffer , 10);
//    printf("mode:%p \n" , buffer[7]);
#if 0   
    while(1)
    {
    I2C_RecvForGeneral(I2C3 ,SLAVEADDR , ADDR , 1 , buffer, 10);
    printf("x : %3d  y:%3d  z:%3d \n" , buffer[0],buffer[1],buffer[2]);
    delay(1000*10);
    }
#endif

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
    //
    if(clear_external_irq(25))
    {
        //注意:一定要去读0x3的寄存器，来恢复中断  ....
        I2C_Recv(I2C3 , SLAVEADDR , buffer , 4);
        printf("hello irq is comming ... \n");
    }
#if 0
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

#endif

}
void Fiq(void)
{
    printf("this is Fiq... \n");   
}

