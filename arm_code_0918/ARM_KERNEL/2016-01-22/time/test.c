
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


static int __init test_init(void)
{
#if 0
    struct timespec  time ; 
    struct tm        tt ; 

    time = current_kernel_time();

    printk("sec : %ld  nsec:%ld \n" ,  time.tv_sec , time.tv_nsec);

    time_to_tm(time.tv_sec , 0 , &tt);

    printk("year:%ld month:%d day:%d  hour:%d min:%d sec:%d \n" , tt.tm_year+1900 , tt.tm_mon +1, tt.tm_mday , tt.tm_hour , tt.tm_min , tt.tm_sec);
    
#endif

    struct timeval tv ; 
    struct tm        tt ; 
    
    do_gettimeofday(&tv);
    time_to_tm(tv.tv_sec , 0 , &tt);


    printk("year:%ld month:%d day:%d  hour:%d min:%d sec:%d \n" , tt.tm_year+1900 , tt.tm_mon +1, tt.tm_mday , tt.tm_hour , tt.tm_min , tt.tm_sec);






    return 0 ;
}


static void __exit test_exit(void)
{

}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");
