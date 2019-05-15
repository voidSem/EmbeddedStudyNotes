
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/atomic.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/kernel.h>
#include <linux/slab.h>

struct platform_object {
    struct platform_device pdev;
    char name[1];
};

void my_release(struct device *dev);
static struct platform_device *pdev = NULL ; 
static struct platform_device_id id_table[] = {
    {"myplatform"},
};

int myprobe(struct platform_device *dev);
//1.平台驱动结构体
static struct platform_driver mydriver = {
    .probe = myprobe,
    .driver = {
    .name = "mydriver",
    },
    .id_table = id_table ,
};

int myprobe(struct platform_device *dev)
{
    printk("myplatform driver probe ... \n");
    return 0 ; 
}   

#if 0
static struct platform_device  pdev = {
    .name = "myplatform",
    .id = 100 ,
    .dev = {
        .release = my_release ,
    },
};
#endif

void my_release(struct device *dev)
{
    printk("this is platform device release \n");
}

static int __init test_init(void)
{
    int ret ; 
    //1.对平台类设备进行分配空间
#if 1      
    pdev = platform_device_alloc("myplatform" ,  100);
    if(IS_ERR(pdev))
    {
        ret = PTR_ERR(pdev);
        goto  OUT ; 
    }
#endif
     
//    arch_setup_pdev_archdata(pdev);
    platform_device_add(pdev);

    //注册平台驱动 
    ret = platform_driver_register(&mydriver);  
    if(ret < 0)
    {
        goto OUT ; 
    }


#if 0
    //2.平台设备注册
    ret = platform_device_register(&pdev);
    if(ret < 0)
    {
        goto OUT ; 
    }

#endif

    return  0; 
OUT:
    return ret ; 
}


static void __exit test_exit(void)
{
    struct platform_object *op = NULL ; 
    platform_driver_unregister(&mydriver);

    platform_device_del(pdev);
    op =  container_of(pdev , struct platform_object , pdev );
    kfree(op);
    
//    platform_device_unregister(&pdev);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");
