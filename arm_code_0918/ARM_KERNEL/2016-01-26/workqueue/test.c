
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

#include <asm/uaccess.h>
#include <asm/io.h>

#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

//IRQ_EINT(26)
//所有中断号的一个表　　用来中断注册
unsigned int irqs[] = {
    IRQ_EINT(26),
    IRQ_EINT(27),
    IRQ_EINT(28),
    IRQ_EINT(29)
};
//按键值对应字符表
unsigned int Key_Code[] = {
KEY_A,
KEY_B,
KEY_C,
KEY_D
};

//中断　　　上文　　　　下文
//tasklet    workqueue
//tasklet   是不可以阻塞
//workqueue 可以阻塞　
//上文主要是来处理比较急的事情

//中断处理函数　　　上文　　处理一些很急的事情
//任务调度　　　　　下文    处理一些比较缓的事情 等待到CPU空闲时处理  

static struct workqueue_struct *wq = NULL; 
void do_irq_work(struct work_struct *work);
DECLARE_WORK(myworkqueue , do_irq_work);



//***************************************
//中断处理层
irqreturn_t do_irq(int irqno , void *dev_id)
{
    int key ; 
    key = irqno - irqs[0]  ;  //按键的值
    
    //2.排队
    queue_work(wq , &myworkqueue);

    schedule_work(&myworkqueue);

    return  IRQ_HANDLED ;  //返回事件已经处理
}

void do_irq_work(struct work_struct *work)
{
    printk("hello world1\n");
    ssleep(1);
    printk("hello world2\n");
}

//***************************************
//文件系统操作层

static int myopen(struct inode *inode , struct file *filp)
{
    return 0 ;
}

static ssize_t myread(struct file *filp , char __user *buffer , size_t len , loff_t *loff)
{
    return 0 ; 
}

static long myioctl(struct file *filp , unsigned int cmd , unsigned long arg)
{
    printk("this is in ioctl ... \n");
    return 0 ; 
}

static int myclose(struct inode *inode , struct file *filp)
{   
    printk("this is in myclose ... \n");

    return 0 ;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open  = myopen,
    .read  = myread,
    .unlocked_ioctl = myioctl,
    .release = myclose,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR , //杂设备次设备号自动分配
    .name  = "mymisc",
    .fops  = &fops ,
};

//***************************************
//模块入口出口

static int __init test_init(void)
{
    //1.杂设备注册
    int ret ; 
    int i ;  //中断注册
        
    ret = misc_register(&misc);
    if(ret < 0)
    {
        goto OUT ; 
    }
    
    //中断注册
    for(i = 0 ; i < ARRAY_SIZE(irqs) ; i++)
    {
        ret = request_irq(irqs[i] , do_irq , IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING | IRQF_SHARED , "myirq" , (void *)88 );
        if(ret < 0)
        {
            goto OUT1 ;    
        }
    }

    //１.创建workqueue
    wq = create_workqueue("mywork");

    return 0 ; 

OUT1:
    while(i--)   
    {
        free_irq(irqs[i] , (void*)88);
    }
OUT:
    misc_deregister(&misc);

    return ret ; 
}


static void __exit test_exit(void)
{
    int ret ; 
    int i ;
    //1.释放中断   
    for(i = 0 ; i < ARRAY_SIZE(irqs) ; i++)
    {
        free_irq(irqs[i] , (void *)88);   
    }

    //2.解注册杂设备
    ret = misc_deregister(&misc);

}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");



