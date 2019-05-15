
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/mutex.h>
#include <linux/time.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/input.h>  //输出头文件
#include <linux/timer.h>  //定时器的头文件

#include <asm/uaccess.h>
#include <asm/io.h>

#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/list.h>


//模块入口出口
static int __init test_init(void)
{ //current指针是指向当前任务的指针　　  task_struct   tasks内核所有任务总链表
    struct task_struct  *tmp = NULL ;


    list_for_each_entry(tmp , &(current->tasks) , tasks )
    {
        printk("comm:%20s  pid:%8d   \n" , tmp->comm , tmp->pid);   
    }

    //printk("current : %p \n" , current);

    return 0; 
}


static void __exit test_exit(void)
{

}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");



