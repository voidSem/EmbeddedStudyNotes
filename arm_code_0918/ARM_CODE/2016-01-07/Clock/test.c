

#include "res.h"
#include "Uart.h"
#include "rtc.h"
#include "serial.h"

static print_t  printf = (void *)0x43E11A2C; 
static int count = 0 ; 
static unsigned int Code = 0 ; 

void Reset(void);
void Undef(void);
void Svc(unsigned int SystemCallNum);
void PrefetchAbt(void);
void DataAbt(void);
void Irq(void);
void Fiq(void);

int _start(void)
{
    #define    APLL_CON0    *(U32 *)0x10044100
    printf("APLL P : %d  M : %d  S:%d  \n" , 
(APLL_CON0 >> 8)&0x3F ,  (APLL_CON0>>16)&0x3FF , APLL_CON0&0x7);

    #define    MPLL_CON0    *(U32 *)0x10040108
    printf("MPLL P : %d  M : %d  S:%d  \n" , 
(MPLL_CON0 >> 8)&0x3F ,  (MPLL_CON0>>16)&0x3FF , MPLL_CON0&0x7);

    #define    EPLL_CON0    *(U32 *)0x1003C110
    printf("EPLL P : %d  M : %d  S:%d  \n" , 
(EPLL_CON0 >> 8)&0x3F ,  (EPLL_CON0>>16)&0x1FF , EPLL_CON0&0x7);

    #define    VPLL_CON0    *(U32 *)0x1003C120
    printf("VPLL P : %d  M : %d  S:%d  \n" , 
(VPLL_CON0 >> 8)&0x3F ,  (VPLL_CON0>>16)&0x1FF , VPLL_CON0&0x7);

    /*
     *   APLL  MPLL  EPLL VPLL
     *
     * APLL P : 3  M : 175  S:0  
     * MPLL P : 3  M : 100  S:0  
     * EPLL P : 2  M : 64  S:3  
     * VPLL P : 2  M : 72  S:3  
     *
     * FOUT = (MDIV) X FIN/(PDIV X 2^SDIV )
     * EPLL =  64 * 24M /(2*2^3) ==>  96M
     *
     * VPLL =  72 * 24M / (16)   ==>  108M
     *
     * APLL  1.4G
     *
     * MPLL  800M
     *
     * EPLL  96M
     *
     * VPLL  108M
     *
     * */

    //DOMAIN 1
    //MUX
    //MUXAPLL  MUXCORE  MUXHPM  MUXMPLL
    #define    CLK_SRC_CPU    *(U32 *)0x10044200
    
    printf("MUXAPLL : %d  MUXCORE: %d  MUXHPM: %d MUXMPLL: %d \n",
    CLK_SRC_CPU&1 , !!(CLK_SRC_CPU&(1<<16)),!!(CLK_SRC_CPU&(1<<20)),
    !!(CLK_SRC_CPU&(1<<24)));


    //DOMAIN 1
    //DIV

    #define    CLK_DIV_CPU0   *(U32 *)0x10044500
    #define    CLK_DIV_CPU1   *(U32 *)0x10044504
    printf("CORE:%d     COREM0:%d     COREM1:%d     PERIPH:%d      \
\nATB:%d      PCLK_DBUG:%d  APLL:%d       CORE2:%d       \n",
    CLK_DIV_CPU0&0x7 , (CLK_DIV_CPU0>>4)&0x7,(CLK_DIV_CPU0>>8)&0x7,
    (CLK_DIV_CPU0>>12)&0x7,(CLK_DIV_CPU0>>16)&0x7,(CLK_DIV_CPU0>>20)&0x7,
    (CLK_DIV_CPU0>>24)&0x7,(CLK_DIV_CPU0>>28)&0x7);

    printf("COPY:%d     HPM:%d        CORES:%d    \n" , 
    CLK_DIV_CPU1&0x7 ,(CLK_DIV_CPU1>>4)&0x7,(CLK_DIV_CPU1>>8)&0x7);

    /*
     *  ARMCLK         1.4G
     *
     *  SCLK_APLL      466M
     *
     *  PCLK_DBUG      100M
     *
     *  ATCLK -> HCLK  200M
     *
     *  ACLK_COREM0    350M
     *
     *  ACLK_CORES     233M
     *
     *  ACLK_COREM1    175M
     *
     *  ACLK_PERIPH    175M
     *
     *  HCLK_HPM       200M
     * */


    //DOMAIN 3
    //
    //MUX
    printf("*****************************************\n");

    #define    CLK_SRC_TOP0    *(U32 *)0x1003C210
    #define    CLK_SRC_TOP1    *(U32 *)0x1003C214

    printf("ONENAND_1:%d     EPLL:%d  VPLL:%d  ACLK_200:%d  ACLK_100:%d \
    ACLK_160:%d  ACLK_133:%d ONENAND:%d  \n" , !!(CLK_SRC_TOP0&(1<<0)),
    !!(CLK_SRC_TOP0&(1<<4)),!!(CLK_SRC_TOP0&(1<<8)),!!(CLK_SRC_TOP0&(1<<12)),
    !!(CLK_SRC_TOP0&(1<<16)),!!(CLK_SRC_TOP0&(1<<20)),!!(CLK_SRC_TOP0&(1<<24)),
    !!(CLK_SRC_TOP0&(1<<28)));

    printf("ONENAND_1:%d     EPLL:%d  VPLL:%d  ACLK_200:%d  ACLK_100:%d \
    ACLK_160:%d  ACLK_133:%d ONENAND:%d  \n" , (CLK_SRC_TOP0>>0)&0x1,
   (CLK_SRC_TOP0>>4)&0x1, (CLK_SRC_TOP0>>8)&0x1, (CLK_SRC_TOP0>>12)&0x1, (CLK_SRC_TOP0>>16)&0x1, (CLK_SRC_TOP0>>20)&0x1, (CLK_SRC_TOP0>>24)&0x1, (CLK_SRC_TOP0>>28)&0x1
  );

    printf("266_GPS:%d  400_MCUISP:%d MPLL:%d 266_GPS_SUB:%d  \
    200_SUB:%d  400_MCUISP_SUB:%d \n", 

!!(CLK_SRC_TOP1&(1<<4)),!!(CLK_SRC_TOP1&(1<<8)),!!(CLK_SRC_TOP1&(1<<12)),
!!(CLK_SRC_TOP1&(1<<16)),!!(CLK_SRC_TOP1&(1<<20)),!!(CLK_SRC_TOP1&(1<<24))
    
        );


    //DIV
    #define   CLK_DIV_TOP    *(U32 *)0x1003C510
    
    printf("ACLK_200:%d  ACLK_100:%d  ACLK_160:%d  ACLK_133:%d ONENAND:%d    266_GPS:%d  400_MCUISP:%d \n" , CLK_DIV_TOP&0x7 , (CLK_DIV_TOP>>4)&0xF ,   (CLK_DIV_TOP>>8)&0x7 , (CLK_DIV_TOP>>12)&0x7, (CLK_DIV_TOP>>16)&0x7, (CLK_DIV_TOP>>20)&0x7, (CLK_DIV_TOP>>24)&0x7);





    //测试
    //
    #define   CLK_DIV_PERIL0  *(U32 *)0x1003C550
    printf("UART0:%d UART1:%d UART2:%d UART3:%d UART4:%d \n" , 
    CLK_DIV_PERIL0&0xF , (CLK_DIV_PERIL0>>4)&0xF, (CLK_DIV_PERIL0>>8)&0xF, (CLK_DIV_PERIL0>>12)&0xF, (CLK_DIV_PERIL0>>16)&0xF);






#if 0
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
	
	unsigned int tiggermode = 0x4 ; 
	enable_external_irq(tiggermode , 11);

	//configure Trigger pin
	//set GPA1CON as output
	GPA1CON  &= ~(0xf << 20) ; 
	GPA1CON  |= 0x1 << 20 ; 



	while(1)
	{
		GPA1DAT &= ~(1 << 5) ; 
		GPA1DAT |= 1 << 5 ;
		delay(1000*10);
		GPA1DAT &= ~(1 << 5) ;

	//	printf("hello trigger is comming ... \n");
		delay(1000*1000);
	}

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
	
	if(Check_Timer(3))
//	if(TINT_CSTAT & (1 << 5))
	{
		//printf("timer coming ... \n");
		count++ ; 
//		TINT_CSTAT |= 1 << 5 ; 
	}
	#define  GPX1DAT    0x11000C24
	if(clear_external_irq(11))
	{
		if(i ^= 0x1)
		{	
			printf("count:%4d \n" , count);
			Disable_Timer(3);
			count = 0 ; 
		}
		else
		{
			count = 0; 
			Enable_Timer(3);
			printf("hahaha \n");
		}
		//printf("interrupt is comming ... \n");
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

