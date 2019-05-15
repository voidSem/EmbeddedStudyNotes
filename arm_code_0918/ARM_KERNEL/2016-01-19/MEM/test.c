
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
#include <linux/kernel.h>


#define  DEVMAJOR    200
#define  DEVNAME     "MYTEST"
#define  TAIL        0xFF

//*************************************
//数据结构
#define    QUEUESIZE    1024

//如果有指针转化　__force
static  char  Buf[QUEUESIZE] = {0};
static  unsigned int tail = 0; 
//*************************************
//内核态文件系统操作层

static int myopen(struct inode *inode , struct file *filp)
{ //private_data 就是来传输一些私有的数据
    

    filp->private_data  =  inode ; 
//    printk("filp->f_pos:%p  \n" , &(filp->f_pos));

//   printk("private_data:%p \n" , filp->private_data);

    return 0 ; 
}

//read filp 是由open的filp传过来的，但是复制过来的
static ssize_t myread(struct file *filp , char __user *buffer , size_t len , loff_t *loff)
{
//    printk("read pos:%lld \n" , filp->f_pos);
//    printk("loff:%p \n" , loff);
//    filp->f_pos = 200 ;
//    printk("read loff:%lld \n" , *loff);
    int ret ; 
    if(*loff + len <= QUEUESIZE)
    {
        ret = copy_to_user(buffer , Buf+*loff , len) ;
        if(ret != 0)
        {
            goto out ; 
        }
        ret = len ; 
    }else
        ret = 0 ;

    *loff += len ; 

    return ret ;
out:
    return ret ; 
}

static ssize_t mywrite(struct file *filp , const char __user *buffer , size_t len , loff_t *loff)
{
    int ret ; 
//    printk("write pos:%lld \n" , filp->f_pos);
    if(*loff + len >= QUEUESIZE)
    {
        return 0 ; 
    }
    
    ret = copy_from_user(Buf+*loff , buffer , len);
    if(ret != 0)
    {
        goto out; 
    }


    *loff += len ; 
    if(*loff >= tail)
    {
        Buf[*loff] = TAIL ; 
        tail = *loff ; 
    }
    return len; 
out:
    return ret ; 
}

//
//
static loff_t mylseek(struct file *filp , loff_t loff , int whence)
{
    long long  val ; 
//    int  index ; 
    val = filp->f_pos ; 

    switch(whence)
    {
        case SEEK_SET : val = loff ;                break;
        case SEEK_END :    
//        for(index = 0 ; index < QUEUESIZE ; index++)
//        {
//           if(Buf[index] == TAIL) 
//                break;
//        }
//        Buf[index] = 0 ; 
        val = tail + loff ; 
//        Buf[val] = TAIL ;
        break;
        case SEEK_CUR :  
        val += loff ; 
        break;
        default :  break;
    }
    printk("val : %lld loff:%lld  \n" , val , loff);
    filp->f_pos = val ; 

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



