#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("FV");
MODULE_DESCRIPTION("A simple LKM to demonstrate device driver basics");

static int Major;

static int driver_open(struct inode *dev_file, struct file *filp)
{
	printk("dev_nr - open was called!\n");
	return 0;
}

static int driver_close(struct inode *dev_file, struct file *filp)
{
	printk("dev_nr - closed was called!\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close
};

static int __init ModuleInit(void)
{
	printk("Hello kernel!\n");
	Major = register_chrdev(0, "dev_nr", &fops);
	if(Major < 0)
	{
		printk("Could not register dev_nr\n");
		return Major;
	}
	printk("dev_nr registered! Major: %d\n", Major);
	return 0;
}

static void __exit ModuleExit(void)
{
	printk("Goodbye Kernel!\n");
	unregister_chrdev(Major, "dev_nr");
}

module_init(ModuleInit);
module_exit(ModuleExit);
