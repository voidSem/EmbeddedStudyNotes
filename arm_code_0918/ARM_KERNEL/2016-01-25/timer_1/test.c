
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

static struct timer_list  timer ; 

//定时器的回调函数


void  timer_fun(unsigned long data)
{
    mod_timer(&timer , jiffies + HZ);
    printk("hahaha this is in timer ... \n");
}

//模块入口出口

static int __init test_init(void)
{
    init_timer(&timer);
    //setup_timer(&timer ,  timer_fun,88);
    //mod_timer(&timer , jiffies + 5*HZ);
    return 0; 
}


static void __exit test_exit(void)
{
    del_timer(&timer);
}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");



