#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h> /* This module is used to create the device file automatically */
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FV");
MODULE_DESCRIPTION("A simple LKM to serve as a read/write driver");

#define DEV_NAME "devDriver"
#define DRIVER_CLASS "dev_mod_class"

static dev_t Major;
static struct class *my_class;
static struct cdev my_device;
static char buffer[255];
static int buffer_ptr;


static int driver_open(struct inode *dev_file, struct file *filp)
{
	printk("read_write - open was called!\n");
	return 0;
}

static int driver_close(struct inode *dev_file, struct file *filp)
{
	printk("read_write - closed was called!\n");
	return 0;
}

/* This method is called when the device file is read from */
static ssize_t device_read(struct file *filp, char *user_buffer, size_t count, loff_t *offset)
{
	/* Essentially it reads data from an internal buffer and writes it to user space */
	int to_copy, not_copied, delta;
	buffer_ptr = sizeof(buffer);
	to_copy = min(count, buffer_ptr);
	not_copied = copy_to_user(user_buffer, buffer, to_copy);
	delta = to_copy - not_copied;

	return delta;
}

/* This method is called when the device file is written to */
static ssize_t device_write(struct file *filp, const char *user_buffer, size_t count, loff_t *offset)
{
	int to_copy, not_copied, delta;

	to_copy = min(count, sizeof(buffer));
	not_copied = copy_from_user(buffer, user_buffer, to_copy);
	buffer_ptr = to_copy;
	delta = to_copy - not_copied;

	return delta;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.read = device_read,
	.write = device_write
};

static int __init ModuleInit(void)
{
	/* This time we create the device file automatically. But if anything goes wrong along the way, we must abort aand cleanup after ourselves */
	printk("Inserting %s in kernel\n", DEV_NAME);
	if(alloc_chrdev_region(&Major, 0, 1, DEV_NAME) < 0)
	{
		printk("Device number could not be allocated.\n");
		return -1;
	}

	printk("Device number --> Major: %d, Minor: %d\n", Major >> 20, Major && 0xfffff);
	if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
	{
		printk("Device class could not be created.\n");
		goto ClassError;
	}

	if(device_create(my_class, NULL, Major, NULL, DEV_NAME) == NULL)
	{
		printk("Could not create device file.\n");
		goto FileError;
	}
	
	cdev_init(&my_device, &fops);
	if(cdev_add(&my_device, Major, 1) == -1)
	{
		printk("Failed to register device to kernel.\n");
		goto AddError;
	}

	return 0;

	AddError:
		device_destroy(my_class, Major);
	FileError:
		class_destroy(my_class);
	ClassError:
		unregister_chrdev(Major, DEV_NAME);
		return -1;
	return 0;
}

/* When the module is unloaded, we need to undo everything that was done in the init method */
static void __exit ModuleExit(void)
{
	printk("Unloading %s from kernel.\n", DEV_NAME);
	
	cdev_del(&my_device);
	device_destroy(my_class, Major);
	class_destroy(my_class);
	unregister_chrdev(Major, DEV_NAME);
	printk("Successfully removed %s.\n", DEV_NAME);
}

module_init(ModuleInit);
module_exit(ModuleExit);
