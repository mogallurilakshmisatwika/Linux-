#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>

dev_t dev=MKDEV(230,0);

static int __init majorstatic_init(void)
{
printk(KERN_INFO "Module Loaded\n");
register_chrdev_region(dev,1,"UNIVISION");
printk(KERN_INFO "chardevice is registered");
printk(KERN_INFO "Major=%d and Minor=%d\n",MAJOR(dev),MINOR(dev));
return 0;
}

static void __exit majorstatic_exit(void)
{
unregister_chrdev_region(dev,1);
printk(KERN_INFO "Module Unloded");
}

module_init(majorstatic_init);
module_exit(majorstatic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Satwika");
MODULE_DESCRIPTION("allocating major number statically");