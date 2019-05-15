
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

//用户访问头文件
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/err.h>


#define  DEVMAJOR    200
#define  DEVNAME     "MYTEST"


//*************************************
//数据结构
#define    QUEUESIZE    1024

//如果有指针转化　__force
//__initconst   __initdata   
static  char  buffer[QUEUESIZE] = {0};
static  char  tmp[QUEUESIZE] = {0};
static  int   head = 0, tail = 0; 
//头进尾出　

//判断队列是否已经满
static int __must_check is_full(void)
{//返回１为满
    return  (tail == ((head + 1) % QUEUESIZE)) ;   
}

//判断队列是否为空
static int __must_check is_empty(void)
{//返回1为空
    return  (tail == head) ;
}
//入队   注意全部传int
static int __must_check enqueue(int ch)
{
    if(is_full())
        return 1 ; 
    buffer[head] = ch ;
    head = (head + 1) % QUEUESIZE ; 
    return 0 ;
}
//dump_stack
//出队
static int __must_check dequeue(char *val)
{
    if(is_empty())
        return -1 ; 

    *val = buffer[tail] ; 
    tail = (tail + 1) % QUEUESIZE ; 
    return 0 ; 
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
//    printk("this is read .... \n");

    int err ; 
    int i ; 

    //1. 出队
    for(i = 0 ; i < len ; i++)
    {
        if(dequeue(&(tmp[i])))
            break;
    }
    //2.
    err = copy_to_user(buffer , tmp , i);
    if(err != 0)
    {
        err = EINVAL ; 
        goto out ; 
    }

    return i ; 
out:
    return err ; 
}

static ssize_t mywrite(struct file *filp , const char __user *buffer , size_t len , loff_t *loff)
{//copy_from_user
   // printk("this is write ... \n") ; 
    int err ; 
    int i ;
    //1.将运用态地址指针空间转入内核态
    err = copy_from_user(tmp , buffer , len);
    if(err != 0)   
    {
        err = EINVAL ; 
        goto out ; 
    }

    //2.
    for(i = 0 ; i < len ; i++)
    {
        if(enqueue(tmp[i]))
            break;
    }

    return i ; 
out:
    return err ; 
}

static loff_t mylseek(struct file *filp , loff_t loff , int whence)
{
    switch(whence)
    {
        case SEEK_SET :  printk("seek set ... \n"); break;
        case SEEK_END :  printk("seek end ... \n"); break;
        case SEEK_CUR :  printk("seek cur ... \n"); break;
        default :  break;
    }
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
    .llseek = mylseek,
    .release = myclose,
};

//**************************************
//模块入口出口

static int __init test_init(void)
{
    int ret ; 

    //1.注册字符设备
    ret = register_chrdev(DEVMAJOR , DEVNAME , &fops);   

    return 0 ;
}

static void __exit test_exit(void)
{
    unregister_chrdev(DEVMAJOR , DEVNAME);
}

module_init(test_init);
module_exit(test_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("cjy");



