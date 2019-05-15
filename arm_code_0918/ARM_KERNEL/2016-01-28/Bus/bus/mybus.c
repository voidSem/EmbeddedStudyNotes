
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>

//发布已经定义的总线接口让属于它的设备跟驱动挂上来

int mymatch(struct device *dev , struct device_driver *drv)
{

    printk("this is bus match devname:%s \n" , dev_name(dev));

    return 1 ;  //1  匹配成功   0  匹配不成功 
}

struct  bus_type  mybus = {
    .name = "mybus",
    .match = mymatch,
};


static int __init mybus_init(void)
{
    int ret ; 
    printk("this is mybus init ... \n");
    ret = bus_register(&mybus);
    if(ret < 0)
        return ret ; 
    return 0 ; 
}

static void __exit mybus_exit(void)
{
    printk("this is mybus exit ... \n");
    bus_unregister(&mybus);
}

module_init(mybus_init);
module_exit(mybus_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");

EXPORT_SYMBOL(mybus);  //发布内核外部变量

