
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
//时间头文件
#include <linux/time.h>
//jiffies头文件   jiffies 是一种内核计算时间的单位
#include <linux/jiffies.h>

//信号量锁
#include <linux/semaphore.h>

//初始化信号锁
//1
//DEFINE_SEMAPHORE(sem) ;  //默认只有一个
//static  struct semaphore  sem = __SEMAPHORE_INITIALIZER(sem , 10);
//2
static struct semaphore  sem ; 


static int myopen(struct inode *inode ,struct file *filp)
{
    //1.down_interruptible  可中断
    //2.down_killable       可终止
    //3.down_trylock        尝试
    //4.down_timeout        加锁超时
   
    int ret ; 
    ret = down_timeout(&sem , 5*HZ);  //jiffies + 5*HZ  
    //注意时间累加
    if(ret != 0)
    {
        return ret ; 
    }
   // down(&sem);
    printk("this is open ... \n");
    return 0 ;
}

static int myclose(struct inode *inode , struct file *filp)
{
    up(&sem);
    printk("this is close ... \n");
    return 0 ;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = myopen,
    .release = myclose,
};

struct miscdevice  misc = {
    .minor = MISC_DYNAMIC_MINOR ,
    .name = "kitty",
    .fops = &fops ,

};

static int __init test_init(void)
{
    int ret ; 

    ret = misc_register(&misc);
        
    sema_init(&sem  , 10);

    return 0 ;
}


static void __exit test_exit(void)
{
    int ret ; 

    ret = misc_deregister(&misc);
}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");

