
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

static struct task_struct  *task  =  NULL ;

int thread_func(void *data)
{
    while(1)
    {  //判断该任务是否应该结束　　是否接收到结束的信号
        if(kthread_should_stop())
            break;
        printk("hello kitty %d\n" , kthread_should_stop());
        ssleep(1);
    }
    return 0 ; 
}


//模块入口出口
static int __init test_init(void)
{
    int ret ; 
    //1.创建任务   
    task = kthread_create(thread_func , (void *)88 , "hello%d" , 109);
    if(IS_ERR(task))
    {
        ret = PTR_ERR(task);
        goto OUT ; 
    }

    //2.唤醒
    wake_up_process(task);


    return 0; 
OUT:
    return ret ; 
}


static void __exit test_exit(void)
{//停止的时候只是向该任务发送应该停止的信号，并且进入阻塞，等待任务退出之后才会打破阻塞
    kthread_stop(task);
}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");



