
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/mod_devicetable.h>  //这个头文件必须加 要不然会报错

//获取发布的总线信息
extern struct bus_type mybus ; 

struct of_device_id  devid[] = {
    {"mydevice",},
    {"mydevice2",},
    {"mydevice3",},
    {"mydevice4",},
    {"mydevice5",},
    {""},
};

struct device_driver mydriver = {
    .name = "mydriver",
    .bus = &mybus,
    .owner = THIS_MODULE,
    .of_match_table = devid , 
};

static int __init mydriver_init(void)
{
    int ret ; 
    printk("this is driver init ... \n");
    ret = driver_register(&mydriver);
    if(ret < 0)
    {
        return ret ; 
    }
    return 0 ; 
}

static void __exit mydriver_exit(void)
{
    printk("this is driver exit ... \n");
    driver_unregister(&mydriver);
}

module_init(mydriver_init);
module_exit(mydriver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");

