#include <linux/module.h> /* All kernel modules need this include statement*/
#include <linux/init.h> /* This is used for setting up the module_init and module_exit functions*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FV");
MODULE_DESCRIPTION("A simple LKM to demonstrate the build process and inserting/removing kernel modules");

/* This method is called when the module is inserted into the kernel*/
static int __init ModuleInit(void)
{
	printk("Successfully loaded hello world module\n");
	return 0;
}

/* This method is called when the module is removed from the kernel*/
static void __exit ModuleExit(void)
{
	printk("Unloading hello world module\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
