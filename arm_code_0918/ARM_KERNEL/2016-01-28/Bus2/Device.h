
#ifndef _DEVICE_H
#define _DEVICE_H



struct MyDevice 
{
    struct device Device ;   //必须是实体
    struct file_operations fops ; 
    unsigned int GPD0CON ; 
    unsigned int GPM4CON ; 
    unsigned int GPX3CON ; 
    unsigned int irqs[4] ; 
};

#endif  //_DEVICE_H
