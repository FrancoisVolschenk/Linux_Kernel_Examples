#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>

#include "ioctl_test.h"

#define DRIVER_NAME "ioctl_ex"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FV");
MODULE_DESCRIPTION("A simple LKM to demonstrate ioctl basics");

static int Major;

static int driver_open(struct inode *dev_file, struct file *filp)
{
	printk("ioctl - open was called!\n");
	return 0;
}

static int driver_close(struct inode *dev_file, struct file *filp)
{
	printk("ioctl - closed was called!\n");
	return 0;
}

// global variable for reading and writing
int32_t answer = 42;

static long int my_ioctl(struct file * filp, unsigned cmd, unsigned long arg)
{
	struct mystruct test;
	switch(cmd)
	{
		case WR_VALUE:
			if(copy_from_user(&answer, (int32_t *) arg, sizeof(answer)))
			{
				printk("ioctl_ex - Error copying from user space!\n");
			}else
			{
				printk("ioctl_ex - Update the answer to %d\n", answer);
			}
			break;
		case RD_VALUE:
			if(copy_to_user((int32_t *) arg, &answer, sizeof(answer)))
			{
				printk("ioctl_ex - Error copying to user space!\n");
			}else
			{
				printk("ioctl_ex - Written to user space\n");
			}
			break;
		case GREETER:
			if(copy_from_user(&test, (struct mystruct *) arg, sizeof(test)))
			{
				printk("ioctl_ex - Error copying from user space!\n");
			}else
			{
				printk("ioctl_ex - %d Greets to %s\n", test.repeat, test.name);
			}
			break;


	}
	return 0;
}


static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.unlocked_ioctl = my_ioctl
};

static int __init ModuleInit(void)
{
	printk("Hello kernel!\n");
	Major = register_chrdev(0, DRIVER_NAME, &fops);
	if(Major < 0)
	{
		printk("Could not register ioctl_ex\n");
		return Major;
	}
	printk("%s registered! Major: %d\n", DRIVER_NAME, Major);
	return 0;
}

static void __exit ModuleExit(void)
{
	printk("Goodbye Kernel!\n");
	unregister_chrdev(Major, DRIVER_NAME);
}

module_init(ModuleInit);
module_exit(ModuleExit);
