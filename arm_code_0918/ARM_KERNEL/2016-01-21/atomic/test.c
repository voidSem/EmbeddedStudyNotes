
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/atomic.h>
#include <linux/errno.h>
#include <linux/err.h>

//atomic  原子锁只做条件判断， 不会有阻塞效果  

//volatile

//1.告诉编译器此段代码或者是定义不进行任何优化
//static volatile unsigned int *gpd0con = NULL; 
//__asm__ __volatile__ ();  非优化

//2.防况针功能
//对一个结构体内的变量成员取出之后，再取地址再转成volatile * 再取值

//3.全局变量或者共享变量
//比方说:如果在一个中断处理函数内，去访问一个全局的或者是一块共享的数据，该数据一定要用volatile修饰

static int  index = 0 ; 
//创建一个设备节点。只让用户态最多10个人访问，如果超过10个的阻塞或者是打开失败

static atomic_t atomic = ATOMIC_INIT(10);  //初始化一个原子锁变量，初始值为10

static int myopen(struct inode *inode , struct file *filp )
{
    
//    if(atomic_read(&atomic) <= 0)
//        return -ENOLCK ; 
//原子锁减1
//    atomic_sub(1 , &atomic);
    atomic_dec(&atomic);
    printk("this is misc open ... index:%d atomic:%d\n" , index , atomic_read(&atomic));
    return 0 ; 
}

static ssize_t myread(struct file *filp , char __user *buffer , size_t len , loff_t *loff)
{
    printk("this is misc read ... \n");
    return 0 ; 
}

static int myclose(struct inode *inode , struct file *filp)
{
//原子锁加1
//    atomic_add(1 , &atomic);
    atomic_inc(&atomic);
    printk("this is close ... index:%d \n" , index);
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
