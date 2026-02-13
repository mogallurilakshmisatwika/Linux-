#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>

#define MAJOR_NUM 230
#define SIZE 1024

dev_t dev=MKDEV(MAJOR_NUM,0);

static struct cdev my_cdev;
static char kbuf[SIZE];
static int len=0;

static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *off)
{
        if(count>SIZE)
                count=SIZE;
        if(copy_from_user(kbuf,buf,count))
                return -EFAULT;
        len=count;
        printk(KERN_INFO "written bytes=%ld\n",count);
        return count;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *off)
{
        if(*off>0)
                return 0;
        if(copy_to_user(buf,kbuf,len))
                return -EFAULT;
        *off=len;
        return len;
}

static struct file_operations fops=
{
        .owner=THIS_MODULE,
        .write=my_write,
        .read=my_read,
};

static int __init my_init(void)
{
        printk(KERN_INFO "Module Loaded\n");
        register_chrdev_region(dev,1,"rw_driver");
        cdev_init(&my_cdev,&fops);
        cdev_add(&my_cdev,dev,1);
        printk(KERN_INFO "driver loaded major=%d minor=%d\n",MAJOR(dev),MINOR(dev));
        return 0;
}

static void __exit my_exit(void)
{
        printk(KERN_INFO "Module unloaded\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev,1);
        printk(KERN_INFO "driver unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Satwika");
MODULE_DESCRIPTION("read/write char device");
