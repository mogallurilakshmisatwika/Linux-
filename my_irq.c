#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/interrupt.h>

static int my_irq;

static irqreturn_t my_irq_handler(int irq, void *dev_id)
{
    pr_info("IRQ triggered\n");
    return IRQ_HANDLED;
}

static int my_probe(struct platform_device *pdev)
{
    int ret;
    printk(KERN_INFO "Probe called\n");
    my_irq = platform_get_irq(pdev, 0);
    if (my_irq < 0)
    {
        pr_err("Failed to get IRQ\n");
        return my_irq;
    }

    printk(KERN_INFO "IRQ number = %d\n", my_irq);
    ret = request_irq(my_irq, my_irq_handler,0, "my_irq_test", NULL);
    if (ret)
    {
        pr_err("request_irq failed\n");
        return ret;
    }

    return 0;
}

static void my_remove(struct platform_device *pdev)
{
    pr_info("Remove called\n");
    free_irq(my_irq, NULL);
}

static const struct of_device_id my_of_match[] = {
    { .compatible = "mycompany,my-irq" },
    { },
};
MODULE_DEVICE_TABLE(of, my_of_match);

static struct platform_driver my_driver = {
    .probe  = my_probe,
    .remove = my_remove,
    .driver = {
        .name = "my_irq_driver",
        .of_match_table = my_of_match,
    },
};

module_platform_driver(my_driver);

MODULE_LICENSE("GPL");