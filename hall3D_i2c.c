#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/sysfs.h>

#define REG_TO_EXPOSE 0x0C  

struct hall3d_data {
    struct i2c_client *client;
};


static ssize_t reg_value_show(struct device *dev,
                              struct device_attribute *attr,
                              char *buf)
{
    struct i2c_client *client = to_i2c_client(dev);
    int ret;
    u8 val;

    ret = i2c_smbus_read_byte_data(client, REG_TO_EXPOSE);
    if (ret < 0)
        return ret;

    val = (u8)ret;
    return sprintf(buf, "0x%02x\n", val);
}

static DEVICE_ATTR(reg_value, 0444, reg_value_show, NULL);

static int hall3d_probe(struct i2c_client *client)
{
    struct hall3d_data *data;
    struct device *dev = &client->dev;
    const char *label;
    const char *version;
    u32 sample_rate;
    u32 axis[3];

    dev_info(dev, "7SEMI 3D Hall sensor detected at 0x%02x\n", client->addr);

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    data->client = client;
    i2c_set_clientdata(client, data);

    
    if (!device_property_read_string(dev, "label", &label))
        dev_info(dev, "Label: %s\n", label);

    if (!device_property_read_string(dev, "version", &version))
        dev_info(dev, "Version: %s\n", version);

    if (!device_property_read_u32(dev, "sample-rate", &sample_rate))
        dev_info(dev, "Sample Rate: %u\n", sample_rate);

    if (!device_property_read_u32_array(dev, "axis-enable", axis, 3))
        dev_info(dev, "Axis Enable: %u %u %u\n", axis[0], axis[1], axis[2]);

    
    if (device_create_file(dev, &dev_attr_reg_value)) {
        dev_err(dev, "Failed to create reg_value sysfs entry\n");
        return -EIO;
    }

    return 0;
}


static void hall3d_remove(struct i2c_client *client)
{
    device_remove_file(&client->dev, &dev_attr_reg_value);
    dev_info(&client->dev, "7SEMI 3D Hall sensor removed\n");
}


static const struct of_device_id hall3d_of_match[] = {
    { .compatible = "7semi,3d-hall", },
    { }
};
MODULE_DEVICE_TABLE(of, hall3d_of_match);


static const struct i2c_device_id hall3d_id[] = {
    { "hall3d", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, hall3d_id);


static struct i2c_driver hall3d_driver = {
    .driver = {
        .name = "hall3d",
        .of_match_table = hall3d_of_match,
    },
    .probe = hall3d_probe,
    .remove = hall3d_remove,
    .id_table = hall3d_id,
};

module_i2c_driver(hall3d_driver);

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("7SEMI 3D Hall sensor I2C driver with sysfs register access");
MODULE_LICENSE("GPL");
~
~