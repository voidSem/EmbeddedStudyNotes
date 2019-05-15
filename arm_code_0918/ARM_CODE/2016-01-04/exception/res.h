
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

#endif  //_RES_H
