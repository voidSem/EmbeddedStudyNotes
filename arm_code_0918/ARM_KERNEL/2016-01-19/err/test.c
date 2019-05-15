
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

#include <linux/slab.h>

//错误
#include <linux/errno.h>
#include <linux/err.h>
//返回０成功　　　NULL是有不同的定义
//

static int __must_check func(void)
{
    printk("this is function ... \n");
    
    return 0 ; 
}

//__must_check  返回值必须检查
//IS_ERR  判断指针   出错返回１
//IS_ERR_VAL   判断错误号　int  出错返回１
//ERR_PTR      错误号转指针
//PTR_ERR      指针转错误号


static void  __must_check *function(void)
{
    void *ptr = NULL ; 
    
    ptr = kzalloc(SZ_4K , GFP_KERNEL);
    if(ptr == NULL)
    {
        printk("hello world \n");
        return NULL ; 
    }

//    return ERR_PTR(-ENOEXEC);    //出错时要加负号
    return ptr ; 
}

static int __init test_init(void)
{
    int err ; 
    int val ; 
    void *ptrerr = NULL; 
    err = func();
    if(IS_ERR_VALUE(err))  //如果出错返回1
        goto ERR; 
    
    
    ptrerr = function();
    if(IS_ERR(ptrerr))
    {
        err = PTR_ERR(ptrerr);
        goto ERR ; 
    }
    
    val = 10 ; 
    if(unlikely(val))  //如果真就是真　　如果假就是假
    {
        printk("hello kitty ... \n");
    }


//    if(err < 0)
//        goto ERR ; 

    return 0;  //会直接返回给内核日志
ERR:
    return err ; 
}

static void __exit test_exit(void)
{
}

module_init(test_init);
module_exit(test_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("cjy");



