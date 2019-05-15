
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

//自旋锁
#include <linux/spinlock.h>

//static spinlock_t  spin ;
DEFINE_SPINLOCK(spin);


static int myopen(struct inode *inode ,struct file *filp)
{
    spin_lock(&spin);   

    printk("this is open ... \n");
    spin_unlock(&spin);
    return 0 ;
}

static int myclose(struct inode *inode , struct file *filp)
{
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

