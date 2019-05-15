
#ifndef _RES_H
#define _RES_H


#include "bcd.h"

typedef   volatile unsigned int   U32 ; 
typedef   volatile unsigned short U16 ; 
typedef   volatile unsigned char  U8 ;

typedef   int (*print_t)(const char *,...) ; 


#define     SWRESET    *(U32 *)0x10020400
//SFR
//--> led
#define     GPM4CON    *(U32 *)0x110002E0
#define     GPM4DAT	   *(U32 *)0x110002E4
//--> buzzer
#define     GPD0CON    *(U32 *)0x114000A0
#define     GPD0DAT    *(U32 *)0x114000A4
//--> key
#define     GPX3CON    *(U32 *)0x11000C60
#define     GPX3DAT    *(U32 *)0x11000C64

//rtc register
//
#define     rtcbase    0x10070000
#define     RTCCON     *(U32 *)(rtcbase + 0x0040 )
#define     TICCNT     *(U32 *)(rtcbase + 0x0044 )
#define     RTCALM     *(U32 *)(rtcbase + 0x0050 )
#define     ALMSEC     *(U32 *)(rtcbase + 0x0054 )
#define     ALMMIN     *(U32 *)(rtcbase + 0x0058 )
#define     ALMHOUR    *(U32 *)(rtcbase + 0x005C )
#define     ALMDAY 	   *(U32 *)(rtcbase + 0x0060 )
#define     ALMMON 	   *(U32 *)(rtcbase + 0x0064 )
#define     ALMYEAR    *(U32 *)(rtcbase + 0x0068 )
#define     BCDSEC	   *(U32 *)(rtcbase + 0x0070 )
#define     BCDMIN 	   *(U32 *)(rtcbase + 0x0074 )
#define     BCDHOUR    *(U32 *)(rtcbase + 0x0078 )
#define     BCDDAYWEEK *(U32 *)(rtcbase + 0x0080 )
#define     BCDDAY 	   *(U32 *)(rtcbase + 0x007C )
#define     BCDMON 	   *(U32 *)(rtcbase + 0x0084 )
#define     BCDYEAR    *(U32 *)(rtcbase + 0x0088 )

//uart  register
//Uart 0
#define     uartbase   0x13800000
#define     ULCONn     *(U32 *)(uartbase + 0x0000 )
#define     UCONn      *(U32 *)(uartbase + 0x0004 )
#define     UFCONn     *(U32 *)(uartbase + 0x0008 )
#define     UMCONn     *(U32 *)(uartbase + 0x000C )
#define     UTRSTATn   *(U32 *)(uartbase + 0x0010 )
#define     UERSTATn   *(U32 *)(uartbase + 0x0014 )
#define     UFSTATn    *(U32 *)(uartbase + 0x0018 )
#define     UMSTATn    *(U32 *)(uartbase + 0x001C )
#define     UTXHn      *(U32 *)(uartbase + 0x0020 )
#define     URXHn      *(U32 *)(uartbase + 0x0024 )
#define     UBRDIVn    *(U32 *)(uartbase + 0x0028 )
#define     UFRACVALn  *(U32 *)(uartbase + 0x002C )
#define     UINTPn     *(U32 *)(uartbase + 0x0030 )

#define     GPA1CON    *(U32 *)0x11400020



//SFR
//--> led
#define     BGPM4CON    0x110002E0
#define     BGPM4DAT	0x110002E4
//--> buzzer
#define     BGPD0CON    0x114000A0
#define     BGPD0DAT    0x114000A4
//--> key
#define     BGPX3CON    0x11000C60
#define     BGPX3DAT    0x11000C64

int led_all_on(void);
int led_all_off(void);
int led_which_on(int which);
int buzzer_on(void);
int buzzer_off(void);
int key_configure(void);
int get_key(void);
int delay(unsigned int val);
int led_configure(void);
int buzzer_configure(void);
int led_flash(void);
int led_run(void);
void __div0(void);
void enable_mmu(void);

/* 
 *   中断
 * */

#define    BASEICCADDR  0x10480000
#define    BASEICDADDR  0x10490000

void vector_copy(void);
void enable_cpsr_irq(void);
void enable_irq(unsigned int RecvCpu ,unsigned int SendCpu ,unsigned int irqno);
void send_internal_irq(unsigned int irqno);



//**************************************************
//copy from uboot   arch/arm/include/asm/arch-exynos/gpio.h

struct s5p_gpio_bank {
	unsigned int	con;
	unsigned int	dat;
	unsigned int	pull;
	unsigned int	drv;
	unsigned int	pdn_con;
	unsigned int	pdn_pull;
	unsigned char	res1[8];
};

/* functions */
void gpio_cfg_pin(struct s5p_gpio_bank *bank, int gpio, int cfg);
void gpio_direction_output(struct s5p_gpio_bank *bank, int gpio, int en);
void gpio_direction_input(struct s5p_gpio_bank *bank, int gpio);
void gpio_set_value(struct s5p_gpio_bank *bank, int gpio, int en);
unsigned int gpio_get_value(struct s5p_gpio_bank *bank, int gpio);
void gpio_set_pull(struct s5p_gpio_bank *bank, int gpio, int mode);
void gpio_set_drv(struct s5p_gpio_bank *bank, int gpio, int mode);
void gpio_set_rate(struct s5p_gpio_bank *bank, int gpio, int mode);

/* Pin Lever  */
#define  GPIO_HIGHT     1
#define  GPIO_LOW       0


/* Pin configurations */
#define GPIO_INPUT	0x0
#define GPIO_OUTPUT	0x1
#define GPIO_IRQ	0xf
#define GPIO_FUNC(x)	(x)

/* Pull mode */
#define GPIO_PULL_NONE	0x0 
#define GPIO_PULL_DOWN	0x1
#define GPIO_PULL_UP	0x3

/* Drive Strength level */
#define GPIO_DRV_1X	0x0
#define GPIO_DRV_2X	0x1
#define GPIO_DRV_3X	0x2
#define GPIO_DRV_4X	0x3
#define GPIO_DRV_FAST	0x0
#define GPIO_DRV_SLOW	0x1

//ARRAY_SIZE  Definition

# define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))

#define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:-!!(e); }))

#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a),&(a)[0]))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

//IO   
//

/*
 * Generic virtual read/write.  Note that we don't support half-word
 * read/writes.  We define __arch_*[bl] here, and leave __arch_*w
 * to the architecture specific code.
 */

#define __arch_getb(a)			(*(volatile unsigned char *)(a))
#define __arch_getw(a)			(*(volatile unsigned short *)(a))
#define __arch_getl(a)			(*(volatile unsigned int *)(a))

#define __arch_putb(v,a)		(*(volatile unsigned char *)(a) = (v))
#define __arch_putw(v,a)		(*(volatile unsigned short *)(a) = (v))
#define __arch_putl(v,a)		(*(volatile unsigned int *)(a) = (v))



#define __raw_writeb(v,a)		__arch_putb(v,a)
#define __raw_writew(v,a)		__arch_putw(v,a)
#define __raw_writel(v,a)		__arch_putl(v,a)

#define __raw_readb(a)			__arch_getb(a)
#define __raw_readw(a)			__arch_getw(a)
#define __raw_readl(a)			__arch_getl(a)

#define writeb(v,a)			__arch_putb(v,a)
#define writew(v,a)			__arch_putw(v,a)
#define writel(v,a)			__arch_putl(v,a)


#define readb(a)			__arch_getb(a)
#define readw(a)			__arch_getw(a)
#define readl(a)			__arch_getl(a)


#define CON_MASK(x)		(0xf << ((x) << 2))
#define CON_SFR(x, v)		((v) << ((x) << 2))

#define DAT_MASK(x)		(0x1 << (x))
#define DAT_SET(x)		(0x1 << (x))

#define PULL_MASK(x)		(0x3 << ((x) << 1))
#define PULL_MODE(x, v)		((v) << ((x) << 1))

#define DRV_MASK(x)		(0x3 << ((x) << 1))
#define DRV_SET(x, m)		((m) << ((x) << 1))
#define RATE_MASK(x)		(0x1 << (x + 16))
#define RATE_SET(x)		(0x1 << (x + 16))

//interrupt register  irq
//

#define    iccbase    		0x10480000
#define    ICCICR_CPU0	    *(U32 *)(iccbase + 		0x0000)
#define    ICCPMR_CPU0	    *(U32 *)(iccbase + 		0x0004)
#define    ICCBPR_CPU0	    *(U32 *)(iccbase + 		0x0008)
#define    ICCIAR_CPU0	    *(U32 *)(iccbase + 		0x000C)
#define    ICCEOIR_CPU0	    *(U32 *)(iccbase + 	0x0010)
#define    ICCRPR_CPU0	    *(U32 *)(iccbase + 		0x0014)
#define    ICCHPIR_CPU0	    *(U32 *)(iccbase + 	0x0018)
#define    ICCABPR_CPU0	    *(U32 *)(iccbase + 	0x001C)
#define    ICCICR_CPU1	    *(U32 *)(iccbase + 		0x4000)
#define    ICCPMR_CPU1	    *(U32 *)(iccbase + 		0x4004)
#define    ICCBPR_CPU1	    *(U32 *)(iccbase + 		0x4008)
#define    ICCIAR_CPU1	    *(U32 *)(iccbase + 		0x400C)
#define    ICCEOIR_CPU1	    *(U32 *)(iccbase + 	0x4010)
#define    ICCRPR_CPU1	    *(U32 *)(iccbase + 		0x4014)
#define    ICCHPIR_CPU1	    *(U32 *)(iccbase + 	0x4018)
#define    ICCABPR_CPU1	    *(U32 *)(iccbase + 	0x401C)
#define    ICCICR_CPU2	    *(U32 *)(iccbase + 		0x8000)
#define    ICCPMR_CPU2	    *(U32 *)(iccbase + 		0x8004)
#define    ICCBPR_CPU2	    *(U32 *)(iccbase + 		0x8008)
#define    ICCIAR_CPU2	    *(U32 *)(iccbase + 		0x800C)
#define    ICCEOIR_CPU2	    *(U32 *)(iccbase + 	0x8010)
#define    ICCRPR_CPU2	    *(U32 *)(iccbase + 		0x8014)
#define    ICCHPIR_CPU2	    *(U32 *)(iccbase + 	0x8018)
#define    ICCABPR_CPU2	    *(U32 *)(iccbase + 	0x801C)
           
           
           
#define    icdbase          0x10490000
#define    ICDDCR		    *(U32 *)(icdbase + 		0x0000)
#define    ICDICTR		    *(U32 *)(icdbase + 		0x0004)
#define    ICDIIDR		    *(U32 *)(icdbase + 		0x0008)
#define    ICDISR0_CPU0	    *(U32 *)(icdbase + 	0x0080)
#define    ICDISR1		    *(U32 *)(icdbase + 		0x0084)
#define    ICDISR2		    *(U32 *)(icdbase + 		0x0088)
#define    ICDISR3		    *(U32 *)(icdbase + 		0x008C)
#define    ICDISR4		    *(U32 *)(icdbase + 		0x0090)
#define    ICDISER0_CPU0    *(U32 *)(icdbase + 		0x0100)
#define    ICDISER1		    *(U32 *)(icdbase + 	0x0104)
#define    ICDISER2		    *(U32 *)(icdbase + 	0x0108)
#define    ICDISER3		    *(U32 *)(icdbase + 	0x010C)
#define    ICDISER4		    *(U32 *)(icdbase + 	0x0110)
#define    ICDICER0_CPU0    *(U32 *)(icdbase + 		0x0180)
#define    ICDICER1		    *(U32 *)(icdbase + 	0x0184)
#define    ICDICER2		    *(U32 *)(icdbase + 	0x0188)
#define    ICDICER3		    *(U32 *)(icdbase + 	0x018C)
#define    ICDICER4		    *(U32 *)(icdbase + 	0x0190)
#define    ICDISPR0_CPU0    *(U32 *)(icdbase + 		0x0200)
#define    ICDISPR1		    *(U32 *)(icdbase + 	0x0204)
#define    ICDISPR2		    *(U32 *)(icdbase + 	0x0208)
#define    ICDISPR3		    *(U32 *)(icdbase + 	0x020C)
#define    ICDISPR4		    *(U32 *)(icdbase + 	0x0210)
#define    ICDICPR0_CPU0    *(U32 *)(icdbase + 		0x0280)
#define    ICDICPR1		    *(U32 *)(icdbase + 	0x0284)
#define    ICDICPR2		    *(U32 *)(icdbase + 	0x0288)
#define    ICDICPR3		    *(U32 *)(icdbase + 	0x028C)
#define    ICDICPR4		    *(U32 *)(icdbase + 	0x0290)
#define    ICDABR0_CPU0	    *(U32 *)(icdbase + 	0x0300)
#define    ICDABR1		    *(U32 *)(icdbase + 		0x0304)
#define    ICDABR2		    *(U32 *)(icdbase + 		0x0308)
#define    ICDABR3		    *(U32 *)(icdbase + 		0x030C)
#define    ICDABR4		    *(U32 *)(icdbase + 		0x0310)
#define    ICDIPR0_CPU0	    *(U32 *)(icdbase + 	0x0400)
#define    ICDIPR1_CPU0	    *(U32 *)(icdbase + 	0x0404)
#define    ICDIPR2_CPU0	    *(U32 *)(icdbase + 	0x0408)
#define    ICDIPR3_CPU0	    *(U32 *)(icdbase + 	0x040C)
#define    ICDIPR4_CPU0	    *(U32 *)(icdbase + 	0x0410)
#define    ICDIPR5_CPU0	    *(U32 *)(icdbase + 	0x0414)
#define    ICDIPR6_CPU0	    *(U32 *)(icdbase + 	0x0418)
#define    ICDIPR7_CPU0	    *(U32 *)(icdbase + 	0x041C)
#define    ICDIPR8		    *(U32 *)(icdbase + 		0x0420)
#define    ICDIPR9		    *(U32 *)(icdbase + 		0x0424)
#define    ICDIPR10		    *(U32 *)(icdbase + 	0x0428)
#define    ICDIPR11		    *(U32 *)(icdbase + 	0x042C)
#define    ICDIPR12		    *(U32 *)(icdbase + 	0x0430)
#define    ICDIPR13		    *(U32 *)(icdbase + 	0x0434)
#define    ICDIPR14		    *(U32 *)(icdbase + 	0x0438)
#define    ICDIPR15		    *(U32 *)(icdbase + 	0x043C)
#define    ICDIPR16		    *(U32 *)(icdbase + 	0x0440)
#define    ICDIPR17		    *(U32 *)(icdbase + 	0x0444)
#define    ICDIPR18		    *(U32 *)(icdbase + 	0x0448)
#define    ICDIPR19		    *(U32 *)(icdbase + 	0x044C)
#define    ICDIPR20		    *(U32 *)(icdbase + 	0x0450)
#define    ICDIPR21		    *(U32 *)(icdbase + 	0x0454)
#define    ICDIPR22		    *(U32 *)(icdbase + 	0x0458)
#define    ICDIPR23		    *(U32 *)(icdbase + 	0x045C)
#define    ICDIPR24		    *(U32 *)(icdbase + 	0x0460)
#define    ICDIPR25		    *(U32 *)(icdbase + 	0x0464)
#define    ICDIPR26		    *(U32 *)(icdbase + 	0x0468)
#define    ICDIPR27		    *(U32 *)(icdbase + 	0x046C)
#define    ICDIPR28		    *(U32 *)(icdbase + 	0x0470)
#define    ICDIPR29		    *(U32 *)(icdbase + 	0x0474)
#define    ICDIPR30		    *(U32 *)(icdbase + 	0x0478)
#define    ICDIPR31		    *(U32 *)(icdbase + 	0x047C)
#define    ICDIPR32		    *(U32 *)(icdbase + 	0x0480)
#define    ICDIPR33		    *(U32 *)(icdbase + 	0x0484)
#define    ICDIPR34		    *(U32 *)(icdbase + 	0x0488)
#define    ICDIPR35		    *(U32 *)(icdbase + 	0x048C)
#define    ICDIPR36		    *(U32 *)(icdbase + 	0x0490)
#define    ICDIPR37		    *(U32 *)(icdbase + 	0x0494)
#define    ICDIPR38		    *(U32 *)(icdbase + 	0x0498)
#define    ICDIPR39		    *(U32 *)(icdbase + 	0x049C)
#define    ICDIPTR0_CPU0    *(U32 *)(icdbase + 		0x0800)
#define    ICDIPTR1_CPU0    *(U32 *)(icdbase + 		0x0804)
#define    ICDIPTR2_CPU0    *(U32 *)(icdbase + 		0x0808)
#define    ICDIPTR3_CPU0    *(U32 *)(icdbase + 		0x080C)
#define    ICDIPTR4_CPU0    *(U32 *)(icdbase + 		0x0810)
#define    ICDIPTR5_CPU0    *(U32 *)(icdbase + 		0x0814)
#define    ICDIPTR6_CPU0    *(U32 *)(icdbase + 		0x0818)
#define    ICDIPTR7_CPU0    *(U32 *)(icdbase + 		0x081C)
#define    ICDIPTR8		    *(U32 *)(icdbase + 	0x0820)
#define    ICDIPTR9		    *(U32 *)(icdbase + 	0x0824)
#define    ICDIPTR10	    *(U32 *)(icdbase + 		0x0828)
#define    ICDIPTR11	    *(U32 *)(icdbase + 		0x082C)
#define    ICDIPTR12	    *(U32 *)(icdbase + 		0x0830)
#define    ICDIPTR13	    *(U32 *)(icdbase + 		0x0834)
#define    ICDIPTR14	    *(U32 *)(icdbase + 		0x0838)
#define    ICDIPTR15	    *(U32 *)(icdbase + 		0x083C)
#define    ICDIPTR16	    *(U32 *)(icdbase + 		0x0840)
#define    ICDIPTR17	    *(U32 *)(icdbase + 		0x0844)
#define    ICDIPTR18	    *(U32 *)(icdbase + 		0x0848)
#define    ICDIPTR19	    *(U32 *)(icdbase + 		0x084C)
#define    ICDIPTR20	    *(U32 *)(icdbase + 		0x0850)
#define    ICDIPTR21	    *(U32 *)(icdbase + 		0x0854)
#define    ICDIPTR22	    *(U32 *)(icdbase + 		0x0858)
#define    ICDIPTR23	    *(U32 *)(icdbase + 		0x085C)
#define    ICDIPTR24	    *(U32 *)(icdbase + 		0x0860)
#define    ICDIPTR25	    *(U32 *)(icdbase + 		0x0864)
#define    ICDIPTR26	    *(U32 *)(icdbase + 		0x0868)
#define    ICDIPTR27	    *(U32 *)(icdbase + 		0x086C)
#define    ICDIPTR28	    *(U32 *)(icdbase + 		0x0870)
#define    ICDIPTR29	    *(U32 *)(icdbase + 		0x0874)
#define    ICDIPTR30	    *(U32 *)(icdbase + 		0x0878)
#define    ICDIPTR31	    *(U32 *)(icdbase + 		0x087C)
#define    ICDIPTR32	    *(U32 *)(icdbase + 		0x0880)
#define    ICDIPTR33	    *(U32 *)(icdbase + 		0x0884)
#define    ICDIPTR34	    *(U32 *)(icdbase + 		0x0888)
#define    ICDIPTR35	    *(U32 *)(icdbase + 		0x088C)
#define    ICDIPTR36	    *(U32 *)(icdbase + 		0x0890)
#define    ICDIPTR37	    *(U32 *)(icdbase + 		0x0894)
#define    ICDIPTR38	    *(U32 *)(icdbase + 		0x0898)
#define    ICDIPTR39	    *(U32 *)(icdbase + 		0x089C)
#define    ICDICFR0_CPU0    *(U32 *)(icdbase + 		0x0C00)
#define    ICDICFR1_CPU0    *(U32 *)(icdbase + 		0x0C04)
#define    ICDICFR2		    *(U32 *)(icdbase + 	0x0C08)
#define    ICDICFR3		    *(U32 *)(icdbase + 	0x0C0C)
#define    ICDICFR4		    *(U32 *)(icdbase + 	0x0C10)
#define    ICDICFR5		    *(U32 *)(icdbase + 	0x0C14)
#define    ICDICFR6		    *(U32 *)(icdbase + 	0x0C18)
#define    ICDICFR7		    *(U32 *)(icdbase + 	0x0C1C)
#define    ICDICFR8		    *(U32 *)(icdbase + 	0x0C20)
#define    ICDICFR9		    *(U32 *)(icdbase + 	0x0C24)
#define    SPI_STATUS0	    *(U32 *)(icdbase + 		0x0D04)
#define    SPI_STATUS1	    *(U32 *)(icdbase + 		0x0D08)
#define    SPI_STATUS2	    *(U32 *)(icdbase + 		0x0D0C)
#define    SPI_STATUS3	    *(U32 *)(icdbase + 		0x0D10)
#define    ICDSGIR		    *(U32 *)(icdbase + 		0x0F00)
#define    ICDISR0_CPU1	    *(U32 *)(icdbase + 	0x4080)
#define    ICDISER0_CPU1    *(U32 *)(icdbase + 		0x4100)
#define    ICDICER0_CPU1    *(U32 *)(icdbase + 		0x4180)
#define    ICDISPR0_CPU1    *(U32 *)(icdbase + 		0x4200)
#define    ICDICPR0_CPU1    *(U32 *)(icdbase + 		0x4280)
#define    ICDABR0_CPU1	    *(U32 *)(icdbase + 	0x4300)
#define    ICDIPR0_CPU1	    *(U32 *)(icdbase + 	0x4400)
#define    ICDIPR1_CPU1	    *(U32 *)(icdbase + 	0x4404)
#define    ICDIPR2_CPU1	    *(U32 *)(icdbase + 	0x4408)
#define    ICDIPR3_CPU1	    *(U32 *)(icdbase + 	0x440C)
#define    ICDIPR4_CPU1	    *(U32 *)(icdbase + 	0x4410)
#define    ICDIPR5_CPU1	    *(U32 *)(icdbase + 	0x4414)
#define    ICDIPR6_CPU1	    *(U32 *)(icdbase + 	0x4418)
#define    ICDIPR7_CPU1	    *(U32 *)(icdbase + 	0x441C)
#define    ICDIPTR0_CPU1    *(U32 *)(icdbase + 		0x4800)
#define    ICDIPTR1_CPU1    *(U32 *)(icdbase + 		0x4804)
#define    ICDIPTR2_CPU1    *(U32 *)(icdbase + 		0x4808)
#define    ICDIPTR3_CPU1    *(U32 *)(icdbase + 		0x480C)

//外部中断

#define     extbase           0x11000000
#define     EXT_INT40_CON     *(U32 *)( extbase + 0x0E00)
#define     EXT_INT41_CON     *(U32 *)( extbase + 0x0E04)
#define     EXT_INT42_CON     *(U32 *)( extbase + 0x0E08)
#define     EXT_INT43_CON     *(U32 *)( extbase + 0x0E0C)
#define     EXT_INT40_FLTCON0 *(U32 *)( extbase + 0x0E80)
#define     EXT_INT40_FLTCON1 *(U32 *)( extbase + 0x0E84)
#define     EXT_INT41_FLTCON0 *(U32 *)( extbase + 0x0E88)
#define     EXT_INT41_FLTCON1 *(U32 *)( extbase + 0x0E8C)
#define     EXT_INT42_FLTCON0 *(U32 *)( extbase + 0x0E90)
#define     EXT_INT42_FLTCON1 *(U32 *)( extbase + 0x0E94)
#define     EXT_INT43_FLTCON0 *(U32 *)( extbase + 0x0E98)
#define     EXT_INT43_FLTCON1 *(U32 *)( extbase + 0x0E9C)
#define     EXT_INT40_MASK    *(U32 *)( extbase + 0x0F00) 
#define     EXT_INT41_MASK    *(U32 *)( extbase + 0x0F04) 
#define     EXT_INT42_MASK    *(U32 *)( extbase + 0x0F08) 
#define     EXT_INT43_MASK    *(U32 *)( extbase + 0x0F0C) 
#define     EXT_INT40_PEND    *(U32 *)( extbase + 0x0F40) 
#define     EXT_INT41_PEND    *(U32 *)( extbase + 0x0F44) 
#define     EXT_INT42_PEND    *(U32 *)( extbase + 0x0F48) 
#define     EXT_INT43_PEND    *(U32 *)( extbase + 0x0F4C) 










#endif  //_RES_H
