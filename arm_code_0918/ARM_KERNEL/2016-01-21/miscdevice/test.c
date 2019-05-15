
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

static int myopen(struct inode *inode , struct file *filp )
{
    printk("this is misc open ... \n");
    return 0 ; 
}

static ssize_t myread(struct file *filp , char __user *buffer , size_t len , loff_t *loff)
{
    printk("this is misc read ... \n");
    return 0 ; 
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open  = myopen,
    .read  = myread,
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
