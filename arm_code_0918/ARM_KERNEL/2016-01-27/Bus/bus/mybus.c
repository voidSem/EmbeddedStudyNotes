
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>

//发布已经定义的总线接口让属于它的设备跟驱动挂上来

struct  bus_type  mybus = {
    .name = "mybus",
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

