
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/mod_devicetable.h>

//发布已经定义的总线接口让属于它的设备跟驱动挂上来

int mymatch(struct device *dev , struct device_driver *drv)
{
    //1.获取该驱动支持设备列表 然后将列表中的名字或者类型 跟设备进对匹配
    //如果匹配成功则返回1 匹配不成功则返回0
    int i ;
    const struct of_device_id *id =   drv->of_match_table ; 
    for(i = 0 ;   ; i++)  //如果这样写就必须要驱列表中规定退出方式
    {
        if(id[i].name[0] != '\0')
        {
            if(strcmp(id[i].name , dev_name(dev)) == 0)
                return 1 ;  //匹配成功 
        }
        else
            break;

    }

    printk("this is bus match devname:%s \n" , dev_name(dev));

    return 0 ;  //1  匹配成功   0  匹配不成功 
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




