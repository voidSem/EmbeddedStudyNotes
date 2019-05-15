
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
#include <linux/types.h>



//#define BIT_MASK(nr)        (1UL << ((nr) % BITS_PER_LONG))
//#define BIT_WORD(nr)        ((nr) / BITS_PER_LONG)




static int __init test_init(void)
{
    int ret = 0xFFFF; 

    //置位  将第20位置成1
    __set_bit(20 ,(volatile unsigned long *)&ret);

    __clear_bit(20 , (volatile unsigned long *)&ret);
    printk("ret : %#x \n" , ret);

    return 0 ; 
}


static void __exit test_exit(void)
{

}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");

