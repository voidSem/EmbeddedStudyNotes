
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>

//获取发布的总线信息
extern struct bus_type mybus ; 

struct device mydevice2 = {
    .init_name = "mydevice2",
    .bus = &mybus,
};


static int __init mydevice_init(void)
{
    int ret ; 
    printk("this is device init ... \n");
    ret = device_register(&mydevice2);
    if(ret < 0)
    {
        return ret ; 
    }
    return 0 ; 
}

static void __exit mydevice_exit(void)
{
    device_unregister(&mydevice2);
    printk("this is device exit ... \n");
}

module_init(mydevice_init);
module_exit(mydevice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");

