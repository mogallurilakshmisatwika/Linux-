#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/io.h>
#include<linux/platform_device.h>

static struct platform_device *my_pdev;
static void __iomem *base;

static struct resource my_resource[]={
        {
                .start=0x10000000,
                .end=0x1000000F,
                .flags=IORESOURCE_MEM,
                .name="fake_mem",
        },
};

static u32 my_read_reg(u32 offset)
{
        return readl(base+offset);
}
static void my_write_reg(u32 offset,u32 value)
{
        writel(value,base+offset);
}
static int my_probe(struct platform_device *pdev)
{
        struct resource *res;
        dev_info(&pdev->dev, "probe called\n");
        res=platform_get_resource(pdev,IORESOURCE_MEM,0);
        if(!res)
        {
                dev_err(&pdev->dev,"failed to get memory resource\n");
                return -EINVAL;
        }
        base=devm_ioremap_resource(&pdev->dev,res);
        if(IS_ERR(base))
        {
                dev_err(&pdev->dev,"ioremap failed\n");
                return PTR_ERR(base);
        }
        dev_info(&pdev->dev,"memory mapped sucessfully\n");
        my_write_reg(0x0,0x12345678);
        dev_info(&pdev->dev,"register value:0x%x\n",my_read_reg(0x0));
        return 0;
}

static int my_remove(struct platform_device *pdev)
{
        dev_info(&pdev->dev,"remove called\n");
        return 0;
}
static struct platform_driver my_driver={
        .probe=my_probe,
        .remove=my_remove,
        .driver={
                .name="minimal_driver",
        },
};
static int __init my_init(void)
{
        int ret;
        pr_info("driver init\n");
        ret=platform_driver_register(&my_driver);
        if(ret)
                return ret;
        my_pdev=platform_device_register_simple("minimal_driver",-1,my_resource,1);
        return 0;
}

static void __exit my_exit(void)
{
        pr_info("driver exit\n");
        platform_device_unregister(my_pdev);
        platform_driver_unregister(&my_driver);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
