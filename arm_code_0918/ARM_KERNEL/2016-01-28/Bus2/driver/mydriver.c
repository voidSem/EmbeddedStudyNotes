
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/mod_devicetable.h>  //这个头文件必须加 要不然会报错
#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "../Device.h"

//获取发布的总线信息
extern struct bus_type mybus ; 

struct of_device_id  devid[] = {
    {"KAIFA_0918",},
    {"mydevice2",},
    {"mydevice3",},
    {"mydevice4",},
    {"mydevice5",},
    {""},
};

static volatile unsigned int __iomem *gpdcon = NULL ;
static volatile unsigned int __iomem *gpmcon = NULL ;
static volatile unsigned int __iomem *gpxcon = NULL ;
static struct MyDevice *MDevice = NULL ; 

int myprobe(struct device *dev);
int myremove(struct device *dev);

struct device_driver mydriver = {
    .name = "mydriver",
    .bus = &mybus,
    .owner = THIS_MODULE,
    .of_match_table = devid , 
    .probe = myprobe ,
    .remove = myremove,
};


//*****************************************
//硬件访问层

static void led_on(void)
{
    unsigned int val ; 
    val = ioread32(gpmcon + 1);
    val &= ~0xF ; 
    iowrite32(val , gpmcon + 1);
}

static void led_off(void)
{
    unsigned int val ; 
    val = ioread32(gpmcon + 1);
    val |= 0xF ; 
    iowrite32(val , gpmcon + 1);
}

static void buzzer_on(void)
{
    unsigned int val ; 
    val = ioread32(gpdcon + 1);
    val |= 0x1 ; 
    iowrite32(val , gpdcon + 1);
}
static void buzzer_off(void)
{
    unsigned int val ; 
    val = ioread32(gpdcon + 1);
    val &= ~0x1 ; 
    iowrite32(val , gpdcon + 1);
    
}

static int Init_Device(void)
{
    unsigned int val  ;

    gpdcon = ioremap(MDevice->GPD0CON , SZ_4K);
    gpmcon = ioremap(MDevice->GPM4CON , SZ_4K);
    gpxcon = ioremap(MDevice->GPX3CON , SZ_4K);

    val = ioread32(gpdcon);
    val &= ~0xF ; 
    val |= 0x1 ; 
    iowrite32(val , gpdcon);


    val = ioread32(gpmcon);
    val &= ~0xFFFF ; 
    val |= 0x1111 ; 
    iowrite32(val , gpmcon);

    buzzer_off();
    led_off();
    return 0 ;
}

static int Exit_Device(void)
{
    iounmap(gpdcon);
    iounmap(gpmcon);
    iounmap(gpxcon);
    return 0 ; 
}

//*****************************************
//中断处理

irqreturn_t do_irq(int irqno , void *data)
{
    int key = irqno - MDevice->irqs[0] ;
    if(ioread32(gpxcon+1) & (1 << (key+2)))
    {//上
        buzzer_off();
        led_off();
    }else
    {//下
        buzzer_on();
        led_on();
    }

    return  IRQ_HANDLED ; 
}

//*****************************************
//文件系统操作层
static int myopen(struct inode *inode ,struct file *filp)
{
    printk("this is in open ... \n");
    return 0 ;
}

static ssize_t myread(struct file *filp , char __user *buffer , size_t len , loff_t *loff)
{
    printk("this is in read ... \n");
    return 0 ; 
}

static ssize_t mywrite(struct file *filp , const char __user *buffer , size_t len , loff_t *loff)
{
    printk("this is in write ... ");
    return 1 ;   //cat
}


static int myclose(struct inode *inode ,struct file *filp)
{
    printk("this is in close ... \n");
    return 0; 
}

int myprobe(struct device *dev)
{
    int i ; 
    int ret ;
    //驱动的main函数
    printk("this is driver probe ... \n");
    //1.获取设备结构体
    MDevice = container_of(dev , struct MyDevice ,  Device);

    //printk("GPD0CON : %#x \n" , MDevice->GPD0CON);
    //1.初始化所有文件操作接口
    MDevice->fops.open = myopen;
    MDevice->fops.read = myread;
    MDevice->fops.write = mywrite;
    MDevice->fops.release = myclose;
    
    //2.初始化硬件 
    Init_Device();
    
    //3.注册中断
    for(i = 0 ;i < ARRAY_SIZE(MDevice->irqs) ; i++)   
    {
        ret = request_irq(MDevice->irqs[i],do_irq,IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING | IRQF_SHARED , "myirq" , (void *)88);  
        if(ret < 0)
            goto OUT ; 
    }
  
    return 0 ; 

OUT:
    while(i--)
        free_irq(MDevice->irqs[i] , (void *)88);
}

int myremove(struct device *dev)
{
    int i ; 
    Exit_Device();
    for(i = 0 ;i < ARRAY_SIZE(MDevice->irqs) ; i++)
    {
        free_irq(MDevice->irqs[i] , (void*)88);
    }

    printk("this is driver remove ... \n");
    return 0 ;
}

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


