
#include "res.h"

static print_t  printf = (void *)0x43E11A2C; 

//enable cpsr
void enable_cpsr_irq(void)
{
    __asm__ __volatile__ (
    "mrs    r0 , cpsr        \n"
    "bic    r0 , #(1 << 7)   \n"
    "msr    cpsr , r0        \n"
    );
}
//拷中断向量表
void vector_copy(void)
{
    extern void start(void);
    extern void end(void);
    //加尾部
    unsigned int offset = (unsigned int )end - (unsigned int )start + 4; 

    //printf("offset : %d \n" , offset);

    memcpy((char *)0x5FFF0000 , (char *)start , offset + 200);

}

/*
 * RecvCpu   哪个CPU配置接收中断
 * SendCpu   发送给哪一个CPU
 * 值都是  0xF 代表 cpu0-cpu3  一位代表一个CPU
 * */
//开启中断号  给ID
void enable_irq(unsigned int RecvCpu ,unsigned int SendCpu ,unsigned int irqno)
{
	int i ; 
	//start
    //enable   irq    internal SGI  ID 6
 	//1>  set GIC  global enable   
    // ICCICR_CPU0 |= 1 ; 
	for(i = 0 ; i < 4 ; i++)
	{
		if(RecvCpu & (1 << i))
		{
			//ICCICR 
			*(U32 *)(BASEICCADDR + i *  0x4000) |= 1 ; 
			//ICCPMR
			*(U32 *)(BASEICCADDR + i *  0x4000+4) |= 0xff ; 
		}
	}

   	//2>  set Distributor Enable
   	//GIC monitors the peripheral interrupt signals and 
   	//forwards pending interrupts to the CPU interfaces. 
   	ICDDCR      |= 1 ; 
	
	//3>  set cpu priority lowest
	//ICCPMR_CPU0 |= 0xff ; 

	
	//***********************************
	//diff ID has diff set
	
	for(i = 0 ; i < 4 ; i++)
	{
		if(RecvCpu & (1 << i))
		{
			//ICDIPTR     (irqno>>2)<<2  irqno&(~0x3)
*(U32 *)(BASEICDADDR + i * 0x4000 + 0x800 + ((irqno>>2)<<2))
		|= (SendCpu & 0xf) << ((irqno & 0x3) * 8) ; 

			//ICDIPR
*(U32 *)(BASEICDADDR + i * 0x4000 + 0x400 + ((irqno>>2)<<2))
		&= ~(0xff << ((irqno & 0x3) * 8) );

*(U32 *)(BASEICDADDR + i * 0x4000 + 0x400 + ((irqno>>2)<<2))
		|= 0x7F << ((irqno & 0x3) * 8);

			//ICDISER
*(U32 *)(BASEICDADDR + i * 0x4000 + 0x100 + ((irqno>>5)<<2))
		|= 1 << (irqno&0x1F);
		}
	}


	//4> send to which cpu
	//ICDIPTR1_CPU0 |= 0x1 << 16 ;   //发给cpu0
	
	//5> 中断ID优先级
	//ICDIPR1_CPU0  &= ~(0xff << 16) ; 
	//ICDIPR1_CPU0  |= 0x7f << 16 ;  //中间值

	//6> 发送的中断ＩＤ
	//ICDISER0_CPU0 |= 1 << 6 ; 

	//end
}
//发送内部中断 
void send_internal_irq(unsigned int irqno)
{
	ICDSGIR = (1 << 16) | (irqno&0xf) ; 
}
//清中断
//cpu 0-3
int clear_irq(unsigned int cpu)
{
	unsigned int id ; 
	//从pending -> active
	//id = ICCIAR_CPU0 & 0x3FF ; 
id = *(U32 *)(BASEICCADDR + (cpu&0x3)*0x4000 + 0xC) & 0x3FF; 
	//从active  -> inactive
	//ICCEOIR_CPU0 = id ; 
*(U32 *)(BASEICCADDR + (cpu&0x3)*0x4000 + 0x10) = id ; 
	return id ; 
}


void enable_external_irq(unsigned int tiggermode , unsigned int eirqno)
{
#if 0
	//将外部中断GPIO配置成中断模式
	GPX3CON |= 0xf << 8 ; 

	
	//1> set trigger mode
	EXT_INT43_CON  &= ~(0xf << 8) ; 
	EXT_INT43_CON  |= 0x4 << 8; 
	
	//2> delay  set filter
	EXT_INT43_FLTCON0 |= 0xff << 16 ; 
	
	//3> enable external irq   set Mask bit
	EXT_INT43_MASK &= ~(1 << 2) ; 
#endif
	//1> 通过外部中断号获取中断ID
	unsigned int irqno ; 
	if(eirqno >= 16)
		irqno = 64 ; 
	else
		irqno = 48 + eirqno ; 

	//2> 开启中断GIC跟cpu部分
	enable_irq(0xF , 0x1 , irqno);


	//3> 配置GPIO为中断模式
	#define  GPXBASE  0x11000C00
	*(U32 *)(GPXBASE + (eirqno>>3)*0x20) |= 
	0xF << ((eirqno&0x7)*4);


	//4> 外部中断控制器  触发模式
	//CON
	#define  EXTCON   0x11000E00
	*(U32 *)(EXTCON + (eirqno>>3)*0x4) &= 
	~(0xF << ((eirqno&0x7)*4));

	*(U32 *)(EXTCON + (eirqno>>3)*0x4) |= 
	(tiggermode&0x7) << ((eirqno&0x7)*4);

	//FILTER
	#define  FILTER   0x11000E80
	*(U32 *)(FILTER + (eirqno>>2)*0x4) |= 
	0xFF << ((eirqno&0x3)*8);

	//MASK
	#define  MASK     0x11000F00
	*(U32 *)(MASK + (eirqno>>3)*0x4) &= 
	~(1 << (eirqno&0x7));
}
//return 0  没有此号中断产生
//return 1  此号外部中断产生并且已经清了中断

int clear_external_irq(unsigned int eirqno)
{
	#define  PEND     0x11000F40
	if((*(U32 *)(PEND + (eirqno>>3)*0x4)) & (1 << (eirqno&0x7)))
	{
	*(U32 *)(PEND + (eirqno>>3)*0x4) |= (1 << (eirqno&0x7));
	return 1 ; 
	}
	return 0 ; 
}
