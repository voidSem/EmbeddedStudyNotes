

#include "res.h"
#include "Uart.h"
#include "rtc.h"
#include "serial.h"
#include "i2c.h"
#include "pwm.h"
#include "bmpheader.h"

#define  RGB(r,g,b)   ((r<<16)|(g<<8)|b)
//#define  RGB(r,g,b)   ((r<<24)|(g<<16)|b<<8)

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
	
#if 0
	//EXT
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

    unsigned int PicAddress = 0x63000000 ; 

    //1.可调时钟
    //打印CLK_DIV_LCD   确定LCD时钟源的频率
    #define    CLK_DIV_LCD    *(U32 *)0x1003C534
    printf("FIMD0_RATIO : %d \n" ,   CLK_DIV_LCD & 0xf);  //15
    //LCD_CLK  ==>   800M / 16 ==> 50M
    CLK_DIV_LCD &= ~0xF ; 
  //  CLK_DIV_LCD |= 7 ;   //100M
    printf("FIMD0_RATIO : %d \n" ,   CLK_DIV_LCD & 0xf);  //15
    
    //2.配置IO
    #define GPF0CON     *(U32 *)0x11400180
    #define GPF1CON     *(U32 *)0x114001A0
    #define GPF2CON     *(U32 *)0x114001C0
    #define GPF3CON     *(U32 *)0x114001E0
    
    GPF0CON  = 0x22222222 ; 
    GPF1CON  = 0x22222222 ; 
    GPF2CON  = 0x22222222 ; 
    GPF3CON  &= ~0xffff   ; 
    GPF3CON  |= 0x2222    ; 

    //3.系统寄存器 开ByPass 门
    #define    LCDBLK_CFG    *(U32 *)(0x10010210)
    LCDBLK_CFG |= 1 << 1 ; 
    

    //4.配置过程
    //1. VIDCON0: Configures video output format 
    //VIDCON0 |= 1 << 18 ;   //set serial format   
    //VIDCON0 |= 2 << 6 ;  //SCLK/3   100M/3  ==> 33.333M

    //2. VIDCON1: Specifies RGB I/F control signal.
        //1> let vclk running
        VIDCON1 |= 1 << 9 ; 
        //2> Fetches Video data at VCLK rising edge
        VIDCON1 |= 1 << 7 ; 
        //3> invert HSYNC   
        VIDCON1 |= 1 << 6 ; 
        //4> invert VSYNC
        VIDCON1 |= 1 << 5 ; 
  
    //3. VIDCON2: Specifies output data format control.
    //set RGB order no dummy no skip
    VIDCON2 |= 1 << 14 ;  //Reserve and must set 1
   
    //4. VIDTCONx: Configures video output timing and determines the size of display.
    VIDTCON0 &= ~0xFFFFFF ;  
    VIDTCON0 |= 10 | (22 << 8) | (13 << 16) ; 
    //VIDTCON0 |= 1 | (20 << 8) | (40 << 16) ; 
    VIDTCON1 &= ~0xFFFFFF ; 
    VIDTCON1 |= 20 | (210 << 8) | (26 << 16) ; 
    //VIDTCON1 |= 1 | (20 << 8) | (20 << 16) ; 
    VIDTCON2 = (479 << 11) | 799 ; 
  
    //5. WINCONx: Specifies each window feature setting.
    WINCON0 |= 0xB << 2 ;  ///RGB 888  24
    //enable
    WINCON0 |= 1 ; 
  
    //6. VIDOSDxA, VIDOSDxB: Specifies window position setting.
    VIDOSD0A = 0 ;   //set left top 0,0
    VIDOSD0B =  (800<<11) | 480 ;  //set right bottom
    //7. VIDOSDxC: Specifies On Screen Display (OSD) size setting.
    VIDOSD0C = 800 * 480 ; 

    //8. VIDWxxADDx: Specifies source image address setting.
    VIDW00ADD0B0 = PicAddress ;
    VIDW00ADD1B0 = PicAddress + 800*480*4 ; 
    VIDW00ADD2   = 800 ; 
   
    //9. SHADOWCON: Specifies shadow control register.
    SHADOWCON |= 1 ; 

    //10. displays enable/disable.  start display controller
    VIDCON0 |= 0x3 ; 


    int row , col ; 
    static int flag = 0x1 ; 
#if 1
    while(1)
    {
        flag ^= 0x1 ; 
        for(row = 0 ; row < 480 ; row++)
        {
            for(col = 0 ; col < 800 ; col++)
            {
//                *(((unsigned int *)PicAddress) + row *800 + col)  = (unsigned int)RGB(0,0,255);
                *(((unsigned int *)PicAddress) + row *800 + col)  = (unsigned int)BMPDATA[row*800+col+flag*(800*480)];
            }
        }
        __udelay(1000*1000);
    }
#endif

  #if 0      
    //    unsigned int rgb = 0xF00 ;
    for(row = 0 ; row < 480 ; row++)
    {
        for(col = 0 ; col < 800 ; col++)
        {

            memcpy(((unsigned char *)PicAddress + 3 * (row *800 + col)), &rgb);
        }
    }
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
	//中断处理函数
	//不可以输出  因为输入输出函数都有效率问题  调试的时候可以打印
	//不可以阻塞，不可以睡眠
	//不可以操作浮点类型的算法
    //

}
void Fiq(void)
{
    printf("this is Fiq... \n");   
}

