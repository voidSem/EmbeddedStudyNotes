

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
    __asm__ __volatile__ (
    "mrs    r0 , cpsr        \n"
    "bic    r0 , #(1 << 7)   \n"
    "msr    cpsr , r0        \n"
    );
    
	//start
    //enable   irq    internal SGI  ID 6
    
 	//1>  set GIC  global enable   
    ICCICR_CPU0 |= 1 ; 

   	//2>  set Distributor Enable
   	//GIC monitors the peripheral interrupt signals and 
   	//forwards pending interrupts to the CPU interfaces. 
   	ICDDCR      |= 1 ; 
	
	//3>  set cpu priority lowest
	ICCPMR_CPU0 |= 0xff ; 

	
	//***********************************
	//diff ID has diff set

	//4> send to which cpu
	ICDIPTR1_CPU0 |= 0x1 << 16 ;   //发给cpu0
	
	//5> 中断ID优先级
	ICDIPR1_CPU0  &= 0xff << 16 ; 
	ICDIPR1_CPU0  |= 0x7f << 16 ;  //中间值

	//6> 发送的中断ＩＤ
	ICDISER0_CPU0 |= 1 << 6 ; 

	//end
	
	//send a internal interrupt
	//inactive -> pending
	ICDSGIR = (1 << 16) | 6 ; 

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
	unsigned int  id ; 
	//清中断
	//从pending -> active
	id = ICCIAR_CPU0 & 0x3FF ; 
	//从active  -> inactive
	ICCEOIR_CPU0 = id ; 
    printf("this is Irq... id:%d \n" , id);   
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
