
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/input.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/bitops.h>  //位操作头文件


//#define BIT_MASK(nr)        (1UL << ((nr) % BITS_PER_LONG))
//#define BIT_WORD(nr)        ((nr) / BITS_PER_LONG)


//事件上报     事件设备     input_dev     i2c_dev
//1.定义一个输入设备结构体
static struct input_dev *mydev = NULL ; 

irqreturn_t  do_irq(int irqno , void *data)
{
    static int flag = 0x1 ; 
    printk("some one press me ... \n");

    //上报事件 
    //1. 上报一个按键事件
    input_report_key(mydev , KEY_A , flag ^= 0x1);


    //2. 同步按键事件   
    input_sync(mydev);


    return   IRQ_HANDLED ; 
}


static int __init test_init(void)
{
    int ret ; 
    //1. 申请中断处理    
    ret = request_irq(IRQ_EINT(26) , do_irq , IRQF_TRIGGER_FALLING | IRQF_SHARED ,  "myirq" , (void *)88);
    if(ret < 0)
    {
        goto OUT ;    
    }
    //2.申请一个输入设备结构体
    mydev = input_allocate_device();
    if(IS_ERR(mydev))
    {
        ret = PTR_ERR(mydev);
        goto OUT1 ; 
    }

    //定义输入设备上报事件的内容    麻烦
    mydev->name = "myinputdev";  //设备名字  创建一个设备节点
    mydev->evbit[0] |=  BIT_MASK(EV_KEY);  //指明上报事件为按键事件
    mydev->keybit[BIT_WORD(KEY_A)] |= BIT_MASK(KEY_A);  //注册一个按键事件


    //3.注册一个输入设备
    ret = input_register_device(mydev);
    if(ret < 0)
    {
        goto OUT2 ; 
    }

    return 0 ; 

OUT2:
    input_free_device(mydev);
OUT1:
    free_irq(IRQ_EINT(26) ,  (void *)88);
OUT:
    return ret ; 
}


static void __exit test_exit(void)
{
    //解注册上报事件设备
    input_unregister_device(mydev);
    //释放已经申请过空间的输入设备结构体 
    input_free_device(mydev);
    //释放中断处理
    free_irq(IRQ_EINT(26) , (void *)88);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");

