
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/mutex.h>
#include <linux/time.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/input.h>  //输出头文件
#include <linux/timer.h>  //定时器的头文件

#include <asm/uaccess.h>
#include <asm/io.h>

#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>

//定时器寄存器的头文件
#include <linux/clk.h>
#include <plat/regs-timer.h>
#include <plat/gpio-cfg.h>
#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>


// one-wire protocol core
#define  SAMPLE_BPS   9600
#define  SLOW_LOOP_FEQ  25
#define  FAST_LOOP_FEQ  60
static unsigned long TCNT_FOR_SAMPLE_BIT;
static unsigned long TCNT_FOR_FAST_LOOP;
static unsigned long TCNT_FOR_SLOW_LOOP;

#define  GPD0CON    0x114000A0
static  volatile unsigned int __iomem *gpdcon = NULL ;

static void Device_init(void)
{
    unsigned int val ; 

    gpdcon = ioremap(GPD0CON , SZ_4K);
    val = ioread32(gpdcon);
    val &= ~0xF ; 
    val |= 0x2 ; 
    iowrite32(val , gpdcon);

}

irqreturn_t  do_timer_irq(int irqno , void *data)
{
    printk("irqno:%d  data:%d \n" , irqno , (int)data);

    return  IRQ_HANDLED ; 
}

static int init_timer_for_1wire(void)
{
	unsigned long tcfg1;
	unsigned long tcfg0;

	unsigned prescale1_value;

	unsigned long pclk;
	struct clk *clk;

	// get pclk
	clk = clk_get(NULL, "timers");
	if (IS_ERR(clk)) {
//		DPRINTK("ERROR to get PCLK\n"); 
        printk("ERROR to get PCLK \n");
		return -EIO;
	}
	clk_enable(clk);

	pclk = clk_get_rate(clk);
	printk("PWM clock = %ld\n", pclk);  //100M

	// get prescaler
	tcfg0 = __raw_readl(S3C2410_TCFG0);
	// we use system prescaler value because timer 4 uses same one
	prescale1_value = (tcfg0 ) & 0xFF;
    printk("prescale1:%d \n" , prescale1_value);

	// calc the TCNT_FOR_SAMPLE_BIT, that is one of the goal
	TCNT_FOR_SAMPLE_BIT = pclk / (prescale1_value + 1) / SAMPLE_BPS - 1;
	TCNT_FOR_FAST_LOOP  = pclk / (prescale1_value + 1) / FAST_LOOP_FEQ - 1;
	TCNT_FOR_SLOW_LOOP  = pclk / (prescale1_value + 1) / SLOW_LOOP_FEQ - 1;

	// select timer 3, the 2rd goal
	tcfg1 = __raw_readl(S3C2410_TCFG1);
	tcfg1 &= ~S3C2410_TCFG1_MUX0_MASK;
	writel(tcfg1, S3C2410_TCFG1);

	printk("TCNT_FOR_SAMPLE_BIT = %ld, TCFG1 = %08x\n",
			TCNT_FOR_SAMPLE_BIT, __raw_readl(S3C2410_TCFG1));
	return 0;
}

static void start_one_wire_session(void)
{
	unsigned long tcon;


	writel(TCNT_FOR_SAMPLE_BIT*1000, S3C2410_TCNTB(0));
    writel(TCNT_FOR_SAMPLE_BIT*500, S3C2410_TCMPB(0));

	// init tranfer and start timer
	tcon = __raw_readl(S3C2410_TCON);
	tcon &= ~(0xF);  
	tcon |= S3C2410_TCON_T0MANUALUPD;
	writel(tcon, S3C2410_TCON);


	tcon |= S3C2410_TCON_T0START;
	tcon |= S3C2410_TCON_T0RELOAD;
    tcon |= S3C2410_TCON_T0INVERT ; 
    tcon |= S3C2410_TCON_T0DEADZONE; 
	tcon &= ~S3C2410_TCON_T0MANUALUPD;
	writel(tcon, S3C2410_TCON);

}



#if 0
static struct irqaction  timer_irq = {
    .name = "myirq",
    .

};
#endif

void start_timer(void)
{
    unsigned int tcon ; 
    
    tcon = __raw_readl(S3C2410_TCON);
    tcon |= S3C2410_TCON_T0START ; 
    writel(tcon , S3C2410_TCON) ;
}

void stop_timer(void)
{
    unsigned int tcon ; 

    tcon = __raw_readl(S3C2410_TCON);
    tcon &= ~S3C2410_TCON_T0START ; 
    writel(tcon , S3C2410_TCON) ;
}


//模块入口出口
static int __init test_init(void)
{
    int ret ; 
    //配置定时器0的时钟
    ret = init_timer_for_1wire();
    if(ret < 0)
    {
        goto ERR ; 
    }
    //注册定时器中断
 
 //   ret = setup_irq(IRQ_TIMER0 ,  &timer_irq);   
    ret = request_irq(IRQ_TIMER0 , do_timer_irq , IRQF_SHARED , "myirq" , (void*)88);
    if(ret < 0)
    {
        goto ERR ; 
    }


    //开始定时器
    start_one_wire_session();
    start_timer();

    //硬件配置
    Device_init();


    return 0; 
ERR:
    return ret ; 
}


static void __exit test_exit(void)
{
    stop_timer();
    free_irq(IRQ_TIMER0 , (void *)88);
}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");



