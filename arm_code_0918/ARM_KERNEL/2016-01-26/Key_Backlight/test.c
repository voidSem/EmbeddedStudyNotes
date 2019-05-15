
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

//向上层回调的结构体
static struct input_event  event ; 

static struct timer_list timer ; 
static volatile unsigned int myflag = 0x1; 


//互斥锁
//
//1. 锁全局　只允许一个人访问此设备　　
DEFINE_MUTEX(Access_Mutex_Lock);
//2. 读阴塞锁
DEFINE_MUTEX(Read_Mutex_Lock);


#define    GPX3CON    0x11000C60

static volatile unsigned int __iomem *gpx3con = NULL ; 

//**************************************
//硬件访问层

static int Device_Init(void)
{
    gpx3con = ioremap(GPX3CON , SZ_4K);
    if(NULL == gpx3con)
        return -ENOMEM ;
    return 0 ; 
}

static int Device_Exit(void)
{
    iounmap(gpx3con);
    return 0 ;
}
//高电平返回1  低电平返回0
static int Key_Status(int key)
{
    unsigned int val ; 

    val = ioread32(gpx3con + 1);
    val >>= 2 ; 
    return  !!(val & (1 << (key%4)))  ; 
}

//读高电平还是低电平

//***************************************
//定时器回调函数
void do_timer(unsigned long data)
{
    myflag = 0x1 ; 

    //中断解锁
    mutex_unlock(&Read_Mutex_Lock);

#if 1   
    if(!Key_Status((int)data))
    {
        myflag = 0 ; 
        timer.expires = jiffies + HZ/5 ; 
        add_timer(&timer);
    }
#endif
}

//***************************************
//中断处理层
irqreturn_t do_irq(int irqno , void *dev_id)
{
    int key ; 
    key = irqno - irqs[0]  ;  //按键的值
    

//    printk("irqno:%d  dev_id:%d key:%d value:%d \n", irqno , (int)dev_id , irqno - IRQ_EINT(26)+1 , Key_Status(key)) ;
    //获取时间
    do_gettimeofday(&(event.time));
    event.value = Key_Status(key);
    event.type = 10 ; 
    event.code = Key_Code[key] ; 

    //中断解锁
    //mutex_unlock(&Read_Mutex_Lock);
    
    //开一个定时器
    if(myflag)
    {
        //if((!Key_Status(key)))
        //{
            myflag = 0 ; 
            timer.expires = jiffies + HZ/20 ;   //0.5s
            timer.data = key ; 
            add_timer(&timer);
        //}else
        //{
        //    mutex_unlock(&Read_Mutex_Lock); 
        //}
    }

    return  IRQ_HANDLED ;  //返回事件已经处理
}

//***************************************
//文件系统操作层

static int myopen(struct inode *inode , struct file *filp)
{
#if 1
    int ret ;
    //获取操作文件的锁
    ret = mutex_lock_interruptible(&Access_Mutex_Lock);
    if(ret < 0)
    {
        return -ENOLCK ; 
    } //以下为获取到锁　　加锁成功
#endif
    //mutex_lock(&Access_Mutex_Lock);
    
    //获取读阻塞的锁 
    ret = mutex_lock_interruptible(&Read_Mutex_Lock);   
    if(ret < 0)
    {
        return -ENOLCK ; 
    }

    printk("this is in open .... \n");
    return 0 ;
}

static ssize_t myread(struct file *filp , char __user *buffer , size_t len , loff_t *loff)
{
    int ret ; 
    //获取读阻塞的锁 
    ret = mutex_lock_interruptible(&Read_Mutex_Lock);   
    if(ret < 0)
    {
        return -ENOLCK ; 
    }

    if(copy_to_user(buffer , &event , sizeof(struct input_event)) != 0)
    {
        return -EINVAL ;   
    }
//    printk("this is in read ... \n");
    return sizeof(struct input_event); 
}

static long myioctl(struct file *filp , unsigned int cmd , unsigned long arg)
{
    printk("this is in ioctl ... \n");
    return 0 ; 
}

static int myclose(struct inode *inode , struct file *filp)
{   
    printk("this is in myclose ... \n");
    //解文件锁
    mutex_unlock(&Access_Mutex_Lock);
    mutex_unlock(&Read_Mutex_Lock);

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
    //设备访问初始化
    ret = Device_Init();
    if(IS_ERR_VALUE(ret))
    {
        goto OUT1 ;    
    }
    
    //初始化定时器
    init_timer(&timer);
    setup_timer(&timer , do_timer , 88);

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

    //3.设备解映射
    Device_Exit();
    //4.删除定时器
    del_timer(&timer);
}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");



