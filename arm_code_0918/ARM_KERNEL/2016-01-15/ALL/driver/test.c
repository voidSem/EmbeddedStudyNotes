
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>  //字符设备头文件 
#include <linux/fs.h>    //文件系统头文件  open  read  write   ioctl
#include <asm/io.h>
#include "../myioctl.h"

#define  MYMAJOR    200   //前期是我们自己指定，必须不可以与系统冲突
#define  MYNAME   "MYTEST"

/*
 *
 *  硬件访问层
 *
 *
 * */


//buzzer register phy address
#define   GPD0CON       0x114000A0

//led
#define   GPM4CON       0x110002E0

//1.变量定义必须初始化
//2.变量声明必须函数最前面

//__iomem  指定该变量存放内存地址空间
volatile unsigned int __iomem *buzzercon= NULL ; 
volatile unsigned int __iomem *ledcon= NULL ; 
//如果出现强转一般情况下写  __force

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
    val |=  0xF ; 
    iowrite32(val , ledcon + 1);
}


static void Init_Device(void)
{
    unsigned int val ; 
    //1.映射内存
    buzzercon = ioremap(GPD0CON ,  SZ_4K);

    ledcon = ioremap(GPM4CON , SZ_4K);

    //2.  set as output
    val = ioread32(buzzercon);   
    val &= ~0xF ; 
    val |= 0x1 ; 
    iowrite32(val , buzzercon);

    val = ioread32(ledcon);
    val &= ~0xFFFF ;    
    val |= 0x1111 ; 
    iowrite32(val , ledcon);

    //3.默认电平
    buzzer_off();
    led_off();
}

static void Exit_Device(void)
{
    iounmap(buzzercon);
    iounmap(ledcon);
}

/*
 *
 *  内核态文件系统操作层
 *
 * */               //major       minor
//字符设置   参数    主设备号    次设备号
//   /proc/devices   显示的是当前系统下的所有设备的名字跟主设备号
//
//[root @ uplooking led ] #ls -l  /dev/console 
//crw-rw----    1 0        0           5,   1 Jan 15 13:43 /dev/console
//[root @ uplooking led ] #mknod /dev/console  c   5   1
//

static int myopen(struct inode *inode , struct file *filp)
{
    printk("this is myopen ... \n");
    return 0 ; 
}

static ssize_t myread(struct file *filp ,char __user *buffer , size_t len , loff_t *loff)
{
    printk("this is in read ... \n");
    return 0 ;  //如果返回1会造成死循环
}

static ssize_t mywrite(struct file *filp , const char __user *buffer , size_t len ,loff_t *loff)
{
    printk("this is in write ... \n");
    return 2 ; 
}

static long myioctl(struct file *filp , unsigned int cmd , unsigned long arg)
{
    switch(cmd)
    {
        case  BUZZER_ON :    buzzer_on();   break;
        case  BUZZER_OFF:    buzzer_off();  break;
        default:    break;
    }
    printk("this is ioctl ... \n");
    return 0 ; 
}

static int myclose(struct inode *inode , struct file *filp)
{
    printk("this is myclose ... \n");
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

/*
 *
 *   模块入口出口
 *
 * */

static int __init test_init(void)
{
    //1.注册一个字符设备
    int ret ;   //返回值   

    ret = register_chrdev(MYMAJOR , MYNAME ,&fops);
    //??

    //2.初始化硬件
    Init_Device();

    printk("hello kernel ... \n");
    return 0 ; 
}

static void __exit test_exit(void)
{
    //1.解注册
    unregister_chrdev(MYMAJOR , MYNAME);
    //2.解物理映射
    Exit_Device();

    printk("good bey kernel ... \n");
}

module_init(test_init);
module_exit(test_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");
MODULE_DESCRIPTION("this is a project");
MODULE_VERSION("1.1.1.0");

