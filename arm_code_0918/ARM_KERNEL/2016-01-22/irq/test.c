
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/atomic.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

#include <linux/delay.h>

#include <linux/spinlock.h>  

//416外部中断的基础值     416 + irqno  ==> 总中断号
DEFINE_SPINLOCK(spin);

irqreturn_t  do_irq(int irqno , void *data)
{
    //注意:  休眠   加阻塞锁  操作大数据  是不允许的
    //ssleep(1);
    //1.测试
    spin_lock(&spin);

    printk("hello irq ... irqno:%d data:%d \n" , irqno , 
    (int)data);

    spin_unlock(&spin);

    return  IRQ_HANDLED ;  //代表中断已经正确处理完成
}

static int __init test_init(void)
{
    int ret ; 
    //查看中断是否已经注册 
    //cat  /proc/interrupts
    //申请中断
    ret = request_irq(IRQ_EINT(26) , do_irq , IRQF_TRIGGER_FALLING | IRQF_SHARED , "myirq" , (void *)88);
    if(ret < 0)
    {
        goto OUT ; 
    }

    return 0 ;
OUT:
    return  ret ; 
}


static void __exit test_exit(void)
{
    int ret ; 
    //一定要释放
    free_irq(IRQ_EINT(26) , (void *)88);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");
