
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/kernel.h>

static struct kset *myclass = NULL; 
//static struct kset mytest ; 
static struct kset *mytest ; 
static struct device  ddev = {
    .init_name = "hellokitty",
    .kobj = {
        .name = "hellokitty",
    },
};


static ssize_t kobj_attr_show(struct kobject *kobj, struct attribute *attr    ,
                  char *buf)
{
    struct kobj_attribute *kattr;
    ssize_t ret = -EIO;

    kattr = container_of(attr, struct kobj_attribute, attr);
    if (kattr->show)
        ret = kattr->show(kobj, kattr, buf);
    return ret;
}

static ssize_t kobj_attr_store(struct kobject *kobj, struct attribute *attr,
                   const char *buf, size_t count)
{
    struct kobj_attribute *kattr;
    ssize_t ret = -EIO;

    kattr = container_of(attr, struct kobj_attribute, attr);
    if (kattr->store)
        ret = kattr->store(kobj, kattr, buf, count);
    return ret;
}


const struct sysfs_ops kobj_sysfs_ops = {
    .show   = kobj_attr_show,
    .store  = kobj_attr_store,
};

static void kset_release(struct kobject *kobj)
{
    struct kset *kset = container_of(kobj, struct kset, kobj);
    pr_debug("kobject: '%s' (%p): %s\n",
         kobject_name(kobj), kobj, __func__);
    kfree(kset);
}


static struct kobj_type myktype = {
    .sysfs_ops  = &kobj_sysfs_ops,
    .release = kset_release,
};  


static int __init test_init(void)
{
    int ret ; 
    struct path Path ; 
    struct dentry *dentry ; 
    dev_t  device_id ; 
    //１./sys文件夹下创建了一个kitty的目录
    myclass = kset_create_and_add("kitty" , NULL , NULL);   
    if(!myclass)
        return  -ENOMEM ; 

    //
    mytest = kzalloc(sizeof(struct kset) , GFP_KERNEL);
    if(!mytest)
    {
        printk("malloc fail");
        goto OUT ; 
    }

    //对kobject进行初始化
    //kset_init(mytest);

    //2.我想在kitty下创建一个目录叫test
    ret = kobject_set_name(&mytest->kobj ,"%s" , "test"); 
    if(ret)
    {
        kfree(mytest);
        goto OUT; 
    }

    mytest->uevent_ops = NULL ;
    mytest->kobj.parent = NULL ;
    mytest->kobj.kset = myclass ; 
    mytest->kobj.ktype = &myktype ; //一定要指定ktype

    ret = kset_register(mytest); 
    if(ret)
    {
        goto OUT ;
    }

    //devtmpfs_create_node(&ddev);

    device_id = MKDEV(100,100);

    dentry = kern_path_create(AT_FDCWD , "hellokitty",&Path,0);
    //vfs_mknod(Path.dentry->d_inode , dentry ,S_IFCHR,device_id);


    return 0 ; 
OUT:
    return ret ;
}

static void __exit test_exit(void)
{
    kset_unregister(mytest);
    kset_unregister(myclass);
}

module_init(test_init);
module_exit(test_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJY");


