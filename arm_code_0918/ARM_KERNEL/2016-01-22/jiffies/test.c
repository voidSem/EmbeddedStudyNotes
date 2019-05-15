
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

#include <linux/delay.h>
//时间头文件
#include <linux/time.h>
//jiffies头文件   jiffies 是一种内核计算时间的单位
#include <linux/jiffies.h>

//时间换算
//1HZ ==> 200jiffies
//1HZ ==> 1s
//1jiffies = 5ms

//1jiffies ==>  1  jiffies_64
//jiffies是一个全局的变量，而且当时间的推移会一直增加

static int __init test_init(void)
{
    
    int value ;
    printk("before ...HZ:%d jiffies:%lu \n" , HZ , jiffies);
    printk("jiffies_64 : %llu \n" , jiffies_64);

    //ssleep(1);   //second  以秒为单位
    //msleep(1);      //ms      以毫秒为单位
    //mdelay(1000);      //ms
    //ndelay(1000*1000);  //最大可以睡眠2ms 
    value = jiffies ; 
    //time_after  如果前面的值比后面的值大的时候返回真
    //while(!time_after(jiffies , value+HZ)) ; 
    //time_before 如果后面的值比前面的值大的时候返回真
    //while(time_before(jiffies , value + HZ)) ; 

    printk("after ...HZ:%d  jiffies:%lu \n" , HZ , jiffies);
    printk("jiffies_64 : %llu \n" , jiffies_64);


    printk("msec:%lu \n" , msecs_to_jiffies(13));
    printk("usec:%lu \n" , usecs_to_jiffies(1000));





    return 0 ;
}


static void __exit test_exit(void)
{

}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");
