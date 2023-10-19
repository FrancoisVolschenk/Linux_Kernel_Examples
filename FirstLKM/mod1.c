#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FV");
MODULE_DESCRIPTION("The first LKM on the pi");

static int __init ModuleInit(void)
{
	printk("Successfully loaded hello world module\n");
	return 0;
}

static void __exit ModuleExit(void)
{
	printk("Unloading hello world module\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
