#include <linux/module.h>
#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FV");
MODULE_DESCRIPTION("A simple LKM to demonstrate parsing a device tree");

static int dt_probe(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);

static struct of_device_id my_driver_ids[] = {
	{
		.compatible = "brightlight,mydev",
	}, {/* sentinel */}
};

MODULE_DEVICE_TABLE(of, my_driver_ids);

static struct platform_driver my_driver = {
	.probe = dt_probe,
	.remove = dt_remove,
	.driver = {
		.name = "my_device_driver",
		.of_match_table = my_driver_ids,
		},
};

static int dt_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	const char *label;
	int my_value, ret;

	printk("dt_probe - Now I am in the probe function\n");

	if(!device_property_present(dev, "label"))
	{
		printk("dt_probe - Error: no property name label\n");
		return -1;
	}

	if(!device_property_present(dev, "my_value"))
	{
		printk("dt_probe - ErrorL no property name my_value\n");
		return -1;
	}

	ret = device_property_read_string(dev, "label", &label);
	if(ret)
	{
		printk("dt_probe - Error: could not read label\n");
		return -1;
	}

	printk("dt_probe - Label: %s\n", label);

	ret = device_property_read_u32(dev, "my_value", &my_value);
	if(ret)
	{
		printk("dt_probe - Error: could not read my_value\n");
		return -1;
	}

	printk("dt_probe - my_value: %d\n", my_value);

	return 0;
}

static int dt_remove(struct platform_device *pdev)
{
	printk("dt_probe - Now I am in  the remove function.\n");
	return 0;
}

static int __init ModuleInit(void)
{
	printk("Loading the driver\n");

	if(platform_driver_register(&my_driver))
	{
		printk("dt_probe - Error: could not register driver.\n");
		return -1;
	}

	return 0;
}

static void __exit ModuleExit(void)
{
	printk("Unregistering the driver\n");
	platform_driver_unregister(&my_driver);
}

module_init(ModuleInit);
module_exit(ModuleExit);
