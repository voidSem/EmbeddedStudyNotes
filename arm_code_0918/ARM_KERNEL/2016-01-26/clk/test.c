
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
#include <linux/clk.h>


//模块入口出口
static int __init test_init(void)
{
    int ret ; 
    unsigned long pclk ; 
    struct clk *clk = NULL; 
    clk = clk_get(NULL , "timers");
    if(IS_ERR(clk))
    {
        ret = PTR_ERR(clk) ; 
        goto OUT ; 
    }

    clk_enable(clk);

    pclk =  clk_get_rate(clk);

    printk("pclk : %ld \n" , pclk);  //100M

    return 0; 
OUT:
    return ret ; 
}


static void __exit test_exit(void)
{

}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");



