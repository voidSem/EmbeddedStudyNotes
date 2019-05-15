
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/kobject.h>
#include <linux/interrupt.h>

#include "../Device.h"

//获取发布的总线信息
extern struct bus_type mybus ; 

#define  DEVNAME   "ALLDEV"
#define  DEVBASE   5
#define  DEVCOUNT  5
static dev_t Device_ID;
//static struct class *cls ; 
static struct cdev  *cd ; 
struct MyDevice  *AllDevice = NULL; 



static void My_Device_Release(struct device *dev)
{
    printk("this is Device release ... \n");
}

static int __init mydevice_init(void)
{
    int ret ; 
    //1.动态分配一个主设备号   已经注册字符设备
    ret = alloc_chrdev_region(&Device_ID , DEVBASE , DEVCOUNT , DEVNAME); 
    if(ret < 0)
    {
        printk("OUT : \n");
        goto OUT ; 
    }

    //2.初始化并且添加设备操作
    cd = cdev_alloc();
    if(IS_ERR(cd))
    {
        printk("OUT :1\n");
        ret = PTR_ERR(cd);
        goto OUT1 ; 
    }

    AllDevice = kzalloc(sizeof(struct MyDevice) , GFP_KERNEL);
    if(IS_ERR(AllDevice))
    {
        printk("OUT :2\n");
        ret = PTR_ERR(AllDevice); //-ENOMEM
        goto OUT2 ; 
    }

    cdev_init(cd ,  &(AllDevice->fops)) ; 

    ret = cdev_add(cd , Device_ID , DEVCOUNT) ; 
    if(ret < 0)
    {
        printk("OUT :3\n");
        goto OUT3 ; 
    }
#if 0
    //3.创建一个类   
    cls = class_create(THIS_MODULE , "ABCDE");
    if(IS_ERR(cls))
    {
        printk("OUT :4\n");
        ret = PTR_ERR(cls);
        goto OUT4 ; 
    }
#endif
#if 0
    //********
    //关联总线 
    AllDevice->Device = kzalloc(sizeof(struct device) , GFP_KERNEL);
    if(IS_ERR(AllDevice->Device))
    {
        printk("OUT :5\n");
        ret = PTR_ERR(AllDevice->Device);
        goto OUT5 ; 
    }
    printk("device:%p \n" , &(AllDevice->Device));
#endif
    //设备初始化   
//    AllDevice->Device->init_name = "hello";
    AllDevice->Device.devt = Device_ID ; 
//    AllDevice->Device->class = cls ; 
    AllDevice->Device.bus   = &mybus ;
    AllDevice->Device.parent = NULL ; 
    AllDevice->Device.release = My_Device_Release ; 
    AllDevice->Device.devt = Device_ID ; 
    //数据初始化
    AllDevice->GPD0CON = 0x114000A0 ; 
    AllDevice->GPM4CON = 0x110002E0 ; 
    AllDevice->GPX3CON = 0x11000C60 ; 
    AllDevice->irqs[0] = IRQ_EINT(26);
    AllDevice->irqs[1] = IRQ_EINT(27);
    AllDevice->irqs[2] = IRQ_EINT(28);
    AllDevice->irqs[3] = IRQ_EINT(29);

    ret = kobject_set_name(&(AllDevice->Device.kobj) , "KAIFA_%04d" , 918);
    if(ret)
    {
        printk("OUT :6\n");
        goto OUT6 ; 
    }

    //最后注册设备
    ret = device_register(&(AllDevice->Device));
    if(ret < 0)
    {
        printk("OUT :7\n");
        goto OUT7 ;    
    }

    return 0 ; 

OUT7:
OUT6:
//    kfree(AllDevice->Device);
//OUT5:
//    class_destroy(cls);
//OUT4:
OUT3:
    kfree(AllDevice);
OUT2:
    cdev_del(cd);
OUT1:
    unregister_chrdev_region(Device_ID , DEVCOUNT);
OUT:
    return  ret ; 
}



static void __exit mydevice_exit(void)
{
    //1.设备删除
    put_device(&(AllDevice->Device));
    device_unregister(&(AllDevice->Device));
//    device_destroy(cls , Device_ID);
//    kfree(AllDevice->Device);
    //2.类删除
//    class_destroy(cls);
    //解注册字符设备
    unregister_chrdev_region(Device_ID , DEVCOUNT);
    cdev_del(cd);
    //解数据
    kfree(AllDevice);
    printk("this is device exit ... \n");
}

module_init(mydevice_init);
module_exit(mydevice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");


