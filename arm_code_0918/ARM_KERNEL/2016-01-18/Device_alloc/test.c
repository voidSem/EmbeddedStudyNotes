
/*
 * 初始化 模块头文件
 * */

#include <linux/init.h>
#include <linux/module.h>

/*
 * 访问IO
 * */

#include <asm/io.h>

//设备注册
#include <linux/cdev.h>
#include <linux/fs.h>

//错误号头文件   
#include <linux/errno.h>

#include "deviceioctl.h"

#define  DEVMINOR    5
#define  DEVCOUNT    5
#define  DEVNAME     "MYTEST"

#define  GPD0CON     0x114000A0
#define  GPM4CON     0x110002E0

static volatile unsigned int __iomem *buzzercon = NULL; 
static volatile unsigned int __iomem *ledcon    = NULL;
static dev_t  Device_ID ; 
static struct cdev  device ; 

//*************************************
//硬件访问

static void buzzer_on(void)
{
    unsigned int val ;  
    val = ioread32(buzzercon + 1);
    val |= 1 ; 
    iowrite32(val , buzzercon + 1);
}

static void buzzer_off(void)
{
    unsigned int val ;  
    val = ioread32(buzzercon + 1);
    val &= ~1 ; 
    iowrite32(val , buzzercon + 1);
}

static void led_on(void)
{
    unsigned int val ; 
    val = ioread32(ledcon + 1);
    val &= ~0xF ;
    iowrite32(val , ledcon + 1);
}

static void led_off(void)
{
    unsigned int val ; 
    val = ioread32(ledcon + 1);
    val |= 0xF ;
    iowrite32(val , ledcon + 1);
}

static int Device_Init(void)
{
    unsigned int val ;

    buzzercon = ioremap(GPD0CON , SZ_4K);
    
    ledcon = ioremap(GPM4CON , SZ_4K);
    
    //初始化  配置输出
    val = ioread32(buzzercon);
    val &= ~0xF ; 
    val |= 1 ; 
    iowrite32(val , buzzercon);

    //led
    val = ioread32(ledcon);
    val &= ~0xFFFF ; 
    val |= 0x1111 ; 
    iowrite32(val , ledcon);

    //默认配置
    buzzer_off();
    led_off();
    return 0 ;
}

static void Device_Exit(void)
{
    iounmap(buzzercon);
    iounmap(ledcon);
}


//*************************************
//内核态文件系统操作层

static int myopen(struct inode *inode , struct file *filp)
{

    printk("this is open ... \n");
    return 0 ; 
}


static ssize_t myread(struct file *filp , char __user *buffer , size_t len , loff_t *loff)
{
    printk("this is read .... \n");
    return 10 ; 
}

static ssize_t mywrite(struct file *filp , const char __user *buffer , size_t len , loff_t *loff)
{
    printk("this is write ... \n") ; 
    return 1 ; 
}

static long myioctl(struct file *filp , unsigned int cmd , unsigned long arg)
{
    switch(cmd)
    {
        case BUZZER_ON :  buzzer_on();   break;
        case BUZZER_OFF:  buzzer_off();  break;
        case LED_ON :     led_on();      break;
        case LED_OFF:     led_off();     break;
        default :  break;
    }
    printk("this is ioctl cmd:%d \n" , cmd);
    return 0 ;
}

static int myclose(struct inode *inode , struct file *filp)
{
    printk("this is close ... \n");
    return 0 ; 
}

struct file_operations  fops = {
    .owner = THIS_MODULE,
    .open  = myopen,
    .read  = myread,
    .write = mywrite,
    .unlocked_ioctl = myioctl,
    .release = myclose,
};

//**************************************
//模块入口出口

static int __init test_init(void)
{
    int err ;

    //region
    //
    //MKDEV     dev_t  val = MKDEV(major , minor);
    //MAJOR     unsigned int major = MAJOR(val)  //获取主设备号
    //MINOR     unsigned int minor = MINOR(val)  //获取次设备号
    //
    
    //1.自动分配一个主设备号
    err = alloc_chrdev_region(&Device_ID , DEVMINOR , DEVCOUNT , DEVNAME);
    if(err < 0)
    {
        goto ERR ; 
    }
    printk("MAJOR:%d \n" , MAJOR(Device_ID));

    //3.设备关联   file_operations fops
    //初始化
    cdev_init(&device , &fops);   

    //添加
    err = cdev_add(&device , Device_ID , DEVCOUNT ) ;
    if(err < 0)
    {
        goto ERR ; 
    }

    //2.初始化设备
    Device_Init();
    return 0 ;

ERR:
    return err ; 
}

static void __exit test_exit(void)
{
    unregister_chrdev_region( Device_ID , DEVCOUNT );
    cdev_del(&device);
    Device_Exit();
}

module_init(test_init);
module_exit(test_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("cjy");


