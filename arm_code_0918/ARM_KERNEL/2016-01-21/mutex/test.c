
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/atomic.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/mutex.h>

//定义一个互斥锁
//1.自己申请空间
//static struct mutex  mut ; 
//2.宏定义
DEFINE_MUTEX(mut);


static int myopen(struct inode *inode , struct file *filp )
{
    //会排队等待，直到获取锁为止
//1.    mutex_lock(&mut);
    int ret ;
    //如果可以上锁就上锁，如果已经有人上锁，就退出,不会等待上锁
//2.    ret = mutex_trylock(&mut);
//3.可被中断的锁 信号中断  ctrl + c  
//如果在加锁等待的时候，进程收到了中断信号，这个锁会自己打破加锁等待然后直接运行 
//    ret = mutex_lock_interruptible(&mut);
   // if(ret == 0)
   // {
   //     printk("lock lock ... \n");
   //     return -ENOLCK ;
   // }

//4. 可以终止
//  比方说,kill  process    会打破该锁，但是不会直接终止整个进程，会直接运行完
    ret = mutex_lock_killable(&mut);

    printk("this is misc open ...\n");
    return 0 ; 
}

static ssize_t myread(struct file *filp , char __user *buffer , size_t len , loff_t *loff)
{
    printk("this is misc read ... \n");
    return 0 ; 
}

static int myclose(struct inode *inode , struct file *filp)
{
    mutex_unlock(&mut);
    printk("this is close ... \n");
    return 0 ; 
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open  = myopen,
    .read  = myread,
    .release = myclose, 
};

struct miscdevice  Device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = "kitty",
    .fops = &fops ,
};

static int __init test_init(void)
{
    int ret ; 
    //2.初始化一下互斥锁 
    mutex_init(&mut);

    ret = misc_register(&Device);

    return 0 ;
}


static void __exit test_exit(void)
{
    int ret ; 

    ret = misc_deregister(&Device);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");
