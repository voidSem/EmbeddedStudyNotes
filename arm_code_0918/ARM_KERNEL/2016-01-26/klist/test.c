
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
#include <linux/list.h>  //内核链表头文件
#include <linux/slab.h>  //申请空间的头文件

struct student 
{
    int a ; 
    int b ; 
    int c ; 
    struct list_head entry ;   //内核链表的结点
};


LIST_HEAD(mylisthead);  //定义一个链表头


//模块入口出口
static int __init test_init(void)
{ //current指针是指向当前任务的指针　　  task_struct   tasks内核所有任务总链表

    struct student *stu = NULL ;
    struct student *tmp = NULL ;   //用来遍历的结点
    struct student *ss = NULL ;    //用来safe遍历
    int i ; 

    for(i = 0 ; i < 10 ; i++)
    {
        //对目标结点进行分配空间
        stu = kzalloc(sizeof(struct student) , GFP_KERNEL);
        if(stu == NULL)
        {
            goto  ERR ; 
        }
        //对目标结点进行初始化
        stu->a = i ;
        stu->b = 0 ; 
        stu->c = 0 ; 
        //对结点内核链表进行初始化
        INIT_LIST_HEAD(&(stu->entry));

        //头插   
        list_add(&(stu->entry) , &mylisthead);
    }


//    list_for_each_entry(tmp , &mylisthead , entry)
    list_for_each_entry_safe(tmp , ss , &mylisthead , entry)
    {
        printk("a : %d b : %d c : %d \n" , tmp->a , tmp->b , tmp->c);
    }

//    list_for_each_entry(tmp , &mylisthead , entry)
#if 1
    list_for_each_entry_safe(tmp , ss , &mylisthead , entry)
    {
        printk("tmp a : %d b : %d c : %d \n" , tmp->a , tmp->b , tmp->c);
        list_del(&(tmp->entry));
        kfree(tmp);
    }
#endif


    return 0; 
ERR:
    return -ENOMEM ; 
}


static void __exit test_exit(void)
{

}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");



