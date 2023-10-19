#include <linux/module.h>
#include <linux/init.h>
#include <linux/serdev.h>
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FV");
MODULE_DESCRIPTION("A simple LKM to read serial data over UART");

static int serdev_echo_probe(struct serdev_device *serdev);
static void serdev_echo_remove(struct serdev_device *serdev);

static struct of_device_id serdev_echo_ids[] = {
	{
		.compatible = "brightlight,serdev",
	}, {/* sentinel */}
};

MODULE_DEVICE_TABLE(of, serdev_echo_ids);

static struct serdev_device_driver serdev_echo_driver = {
	.probe = serdev_echo_probe,
	.remove = serdev_echo_remove,
	.driver = {
		.name = "serdev-echo",
		.of_match_table = serdev_echo_ids,
		},
};

static int serdev_echo_recv(struct serdev_device *serdev, const unsigned char *buffer, size_t size)
{
	printk("serdev: Recieved %ld bytes with \"%s\"\n", size, buffer);
	return serdev_device_write_buf(serdev, buffer, size);
}

static const struct serdev_device_ops serdev_echo_ops = {
	.receive_buf = serdev_echo_recv,
};

static int serdev_echo_probe(struct serdev_device *serdev)
{
	int status;
	printk("serdev_echo - Now I am in the probe function\n");

	serdev_device_set_client_ops(serdev, &serdev_echo_ops);
	status = serdev_device_open(serdev);

	if(status)
	{
		printk("serdev_controller: Error - opening serial port \n");
		return -1;
	}

	serdev_device_set_baudrate(serdev, 2400);
	serdev_device_set_flow_control(serdev, false);
	serdev_device_set_parity(serdev, SERDEV_PARITY_NONE);

	status = serdev_device_write_buf(serdev, "Type something: ", sizeof("Type something: "));
	printk("serdev_echo - Wrote %d bytes\n", status);

	return 0;
}

static void serdev_echo_remove(struct serdev_device *serdev)
{
	printk("serdev_probe - Now I am in  the remove function.\n");
	serdev_device_close(serdev);
}

static int __init ModuleInit(void)
{
	printk("Loading the driver\n");

	if(serdev_device_driver_register(&serdev_echo_driver))
	{
		printk("serdev_probe - Error: could not register driver.\n");
		return -1;
	}

	printk("serdev - done!\n");
	return 0;
}

static void __exit ModuleExit(void)
{
	printk("Unregistering the driver\n");
	serdev_device_driver_unregister(&serdev_echo_driver);
}

module_init(ModuleInit);
module_exit(ModuleExit);
