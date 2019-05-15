
//内核初始化头文件
#include <linux/init.h>
//模块初始化头文件
#include <linux/module.h>

//lsmod        查看当前系统下的所有的模块信息
//insmod xx.ko 将驱动模块xx.ko插入内核      调用入口函数
//rmmod  xx    将当前系统下的xx模块删除     调用出口函数

//内核日志文件   /var/log/message*

//__init  __exit  __initdata  __initconst  ....
// 函数     函数     变量         常量
//如果加上__init  __exit  变量的函数  只跑一次，并且运行完之后会将函数的空间释放掉


//内核入口函数  但是函数名字可变  有且只能有一个入口函数
static int __init test_init(void)
{
    //内核打印   相当于运用层的printf 用法一致
    printk("hello kernel ... \n");
    return 0 ; 
}
//内核出口函数  名字可变     有且只能有一个出口函数
static void __exit test_exit(void)
{
    //出口打印
    printk("Good bey kernel ... \n");
}

//内核入口函数注册    
module_init(test_init);
//内核出口函数注册
module_exit(test_exit);

//内核服从的协议
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1.1.0");  //版本号
MODULE_AUTHOR("CJY");       //作者
MODULE_DESCRIPTION("this is a test"); //模块描述
MODULE_INFO(hahaha , "this is a dog");  //万能

