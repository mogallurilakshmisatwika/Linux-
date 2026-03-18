#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/io.h>
#include<linux/platform_device.h>
#include<linux/of.h>

struct my_driver_data{
        int value;
};

static struct of_device_id my_of_match[]={
        {.compatible="mycompany,mydevice"},
        {}
};
MODULE_DEVICE_TABLE(of,my_of_match);
static int my_helper_function(struct device *dev)
{
        dev_info(dev,"helper function executed\n");
        return 0;
}

static int my_probe(struct platform_device *pdev)
{
        struct my_driver_data *data;
        dev_info(&pdev->dev," probe called\n");
        data=devm_kzalloc(&pdev->dev,sizeof(*data),GFP_KERNEL);
        if(!data)
        {
                dev_err(&pdev->dev,"memory allocation failed\n");
                return -ENOMEM;
        }
        data->value=10;
        platform_set_drvdata(pdev,data);
        my_helper_function(&pdev->dev);
        return 0;
}
static int my_remove(struct platform_device *pdev)
{
        dev_info(&pdev->dev,"remove called\n");
        return 0;
}
static struct platform_driver my_platform_driver={
        .probe=my_probe,
        .remove=my_remove,
        .driver={
                .name="my_platform_driver",
                .of_match_table=my_of_match,
                .owner=THIS_MODULE,
        },
};

static int __init my_init(void)
{
        pr_info("init called\n");
        return platform_driver_register(&my_platform_driver);
}

static void __exit my_exit(void)
{
        pr_info("exit called\n");
        platform_driver_unregister(&my_platform_driver);
}
module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

~