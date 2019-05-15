
#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>

//buzzer register phy address
#define   GPD0CON       0x114000A0

//1.变量定义必须初始化
//2.变量声明必须函数最前面

//__iomem  指定该变量存放内存地址空间
volatile unsigned int __iomem *gpdcon = NULL ; 

static int __init test_init(void)
{
    unsigned int val ; 

    //1.要将低层硬件的物理地址映射到内核态   ioremap
    //2.要将低层硬件的物理地址上的值读出     ioread32
    //3.写入则用                             iowrite32
    //4.解iounmap
    //               offset     size
    gpdcon = ioremap(GPD0CON ,  SZ_4K );

    //set buzzer as output
    //              地址
    val = ioread32(gpdcon);
    
    val &= ~0xF ; 
    val |= 0x1 ; 

    iowrite32(val , gpdcon);

    //buzzer on
    val = ioread32(gpdcon + 1);

    val |= 1 ; 

    iowrite32(val , gpdcon + 1);

    printk("hello kernel ... ");
    return 0 ; 
}



static void __exit test_exit(void)
{
    unsigned int val ; 
    val = ioread32(gpdcon + 1);

    val &= ~1 ; 
    
    iowrite32(val , gpdcon + 1);
    printk("Good bey kernel ... \n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("1.1.1.0");  //版本号
MODULE_AUTHOR("CJY");       //作者
MODULE_DESCRIPTION("this is a test"); //模块描述
MODULE_INFO(hahaha , "this is a dog");  //万能


