#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

static int val=10;

module_param(val,int,0444);

static int __init message_init(void)
{
printk(KERN_INFO "Module loaded\n");

if(val<0)
{
        printk(KERN_ERR "value is negative=%d\n",val);
return -EINVAL;
}
else if(val>0)
{
        printk(KERN_INFO "value is positive=%d\n",val);
}
else
{
        printk(KERN_INFO "value is zero=%d\n",val);
}
return 0;
}


static void __exit message_exit(void)
{
printk(KERN_INFO "module unloaded");
}

module_init(message_init);
module_exit(message_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Satwika");
MODULE_DESCRIPTION("accept a int value and prints message according to it");






