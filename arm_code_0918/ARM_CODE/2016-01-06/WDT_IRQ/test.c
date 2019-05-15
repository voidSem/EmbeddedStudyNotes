

#include "res.h"
#include "Uart.h"
#include "rtc.h"
#include "serial.h"

static print_t  printf = (void *)0x43E11A2C; 

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
	
	//watch dog
	enable_irq(0xF , 0x1 , 75);

	//set watch dog  第一分频 第二分频  决定DownCount的计数心跳
	WTCON = (99 << 8) | (3 << 3) ; //100M/100/128
	//7812HZ  心跳
	//set watch dog interrupt enable
	WTCON |= 1 << 2 ; 

	WTDAT = 7812 * 5 ; 
	
	WTCNT = 7812 * 2 ; 

	//enable watch dog
	WTCON |= 1 | (1 << 5) ; 

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
void Irq(void)
{
	printf("wang wang wang ... \n");
	WTCLRINT = 0 ; 


#if 0
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

