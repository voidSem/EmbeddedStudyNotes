
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/device.h>

dev_t Device_ID ;
struct cdev   Dev; 
//struct class  cls ;   //设备类
struct class *cls = NULL; 
struct device *Device ;  //设备
#define  DEVMINOR   5
#define  DEVCOUNT   5
#define  DEVNAME    "KAIFA"


static int myopen(struct inode *inode  , struct file *filp)
{
    printk("this is myopen ... \n");
    return 0 ;
}

static ssize_t myread(struct file *filp , char __user *buffer , size_t len , loff_t *loff)
{
    printk("this is my read ... \n");
    return 0 ; 
}

struct file_operations  fops = {
    .owner = THIS_MODULE,
    .open = myopen,
    .read = myread,
};

static int __init test_init(void)
{
    //1.allocate an major and minor
    int ret ; 

    ret = alloc_chrdev_region(&Device_ID , DEVMINOR , DEVCOUNT , DEVNAME );
    if(ret < 0)
    {
        goto OUT ;
    }
    
    //2.cdev init
    cdev_init(&Dev , &fops ) ;

    //3.cdev add
    ret = cdev_add(&Dev , Device_ID , DEVCOUNT ) ; 
    if(ret < 0)
    {
        goto OUT ; 
    }

    //以上为字符设备的注册
    //设备是一个总称　包括  字符设备.网络设备.块设备
  
    //1.产生一个类 并且注册一个设备类
    //事先对设备进行简单初始化
    //cls.owner = THIS_MODULE  ; 
    //strcpy(cls.name , "KAIFACLS") ;
 //   ret = class_register(&cls);
    cls = class_create(THIS_MODULE , "KAIFACLS");
    if(IS_ERR(cls))
    {
        ret = PTR_ERR(cls);
        goto OUT ; 
    }
    
    //2.将设备跟类建立关系   创建一个设备   最后操作
    Device = device_create(cls , NULL ,  Device_ID , NULL , 
    "KAIFA_%04d" , 918) ; 
    if(IS_ERR(Device))
    {
        ret = PTR_ERR(Device) ; 
        goto OUT ; 
    }

    return 0 ; 
OUT:
    return ret ; 
}

static void __exit test_exit(void)
{
    //1.释放设备
    device_destroy(cls , Device_ID);

    //2.释放类
    class_destroy(cls);
    
    //3.释放字符设备
    cdev_del(&Dev);
    unregister_chrdev_region(Device_ID , DEVCOUNT);
}

module_init(test_init);
module_exit(test_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");

