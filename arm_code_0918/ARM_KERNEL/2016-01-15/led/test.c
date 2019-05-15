
#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>

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

static int __init test_init(void)
{

    //1.要将低层硬件的物理地址映射到内核态   ioremap
    //2.要将低层硬件的物理地址上的值读出     ioread32
    //3.写入则用                             iowrite32
    //4.解iounmap

    Init_Device();

    buzzer_on();
    led_on();

    printk("hello kernel ... ");
    return 0 ; 
}



static void __exit test_exit(void)
{
    buzzer_off();
    led_off();
    Exit_Device();
    printk("Good bey kernel ... \n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("1.1.1.0");  //版本号
MODULE_AUTHOR("CJY");       //作者
MODULE_DESCRIPTION("this is a test"); //模块描述
MODULE_INFO(hahaha , "this is a dog");  //万能


