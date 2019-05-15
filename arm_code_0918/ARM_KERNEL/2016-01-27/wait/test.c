
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
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct task_struct *ts = NULL; 
static volatile unsigned int fflag = 0 ;
DECLARE_WAIT_QUEUE_HEAD(myhead);

irqreturn_t  do_irq(int irqno , void *data)
{
    //1.测试
    fflag = 1 ;
    printk("irqno:%d\n" , irqno);
    //唤醒等待队列
    wake_up_interruptible(&myhead);

    return  IRQ_HANDLED ;  //代表中断已经正确处理完成 
    } 
static int threadfn(void *data)
{
    
    fflag = 1 ; 
    while(1)
    {   
//        wait_event(myhead , fflag);
        wait_event_interruptible(myhead , fflag);
        if(kthread_should_stop())
            break;
        printk("hello world \n");
        fflag = 0 ; 
    //    ssleep(1);
    }
    return 0 ; 
}

static int __init test_init(void)
{
    int ret ; 

    //申请中断
    ret = request_irq(IRQ_EINT(26) , do_irq , IRQF_TRIGGER_FALLING | IRQF_SHARED , "myirq" , (void *)88);
    if(ret < 0)
    {
        goto OUT ; 
    }
    
    //开任务
    ts = kthread_create(threadfn , (void *)88 , "mythread%d" , 10);
    if(ts == NULL)
    {
        ret = PTR_ERR(ts);
        goto OUT1 ; 
    }

    wake_up_process(ts);

    return 0 ;
OUT1:
    free_irq(IRQ_EINT(26) , (void *)88);
OUT:
    return  ret ; 
}


static void __exit test_exit(void)
{
    int ret ; 

    kthread_stop(ts);

    fflag = 1 ;
    wake_up_interruptible(&myhead);
    //一定要释放
    free_irq(IRQ_EINT(26) , (void *)88);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");
