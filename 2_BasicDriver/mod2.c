#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h> /* This is the file system module, it is used to link the functions in this module with the expected device file operations */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FV");
MODULE_DESCRIPTION("A simple LKM to demonstrate device driver basics");

static int Major; /* This is the number used to uniquely ientify the driver */

/* This method is called when the associated device file is opened */
static int driver_open(struct inode *dev_file, struct file *filp)
{
	printk("dev_nr - open was called!\n");
	return 0;
}

/* This method is called when the associated device file is closed. */
static int driver_close(struct inode *dev_file, struct file *filp)
{
	printk("dev_nr - closed was called!\n");
	return 0;
}

/* This tells the kernel which function in this module to call when certain actions are triggered */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close
};

/* This method is called when the module is loaded into the kernel. It requests a Major number from the kernel */
static int __init ModuleInit(void)
{
	printk("Hello kernel!\n");
	Major = register_chrdev(0, "dev_nr", &fops); /* Providing 0 as the first argument asks the kernel for an unallocated major number. */
	if(Major < 0)/* If the Major number is negative, something went wrong. */
	{
		printk("Could not register dev_nr\n");
		return Major;
	}
	printk("dev_nr registered! Major: %d\n", Major); /* We print this to the kernel logs to know what number to use when creating our device file */
	return 0;
}

/* This method is called when the module is unloaded from the kernel. Anything that was done in the Init method must be undone here. */
static void __exit ModuleExit(void)
{
	printk("Goodbye Kernel!\n");
	unregister_chrdev(Major, "dev_nr"); /* Free up the allocated major number */
}

module_init(ModuleInit);
module_exit(ModuleExit);
