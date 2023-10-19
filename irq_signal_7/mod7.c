#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/sched/signal.h>
#include <linux/ioctl.h>

#define DEV_NAME "gpio_irq_signal"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FV");
MODULE_DESCRIPTION("A simple LKM to send interrupt signals to the user space");

static int Major;
unsigned int irq_number;
static int SignalPin = 17;

//Variables for IOCTL
#define REGISTER_UAPP _IO('R', 'g')
static struct task_struct *task = NULL;

#define SIGNR 44

static irq_handler_t signal_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
	printk("Interrupt was triggered and handled.\n");

	struct siginfo info;
	if(task != NULL)
	{
		memset(&info, 0, sizeof(info));
		info.si_signo = SIGNR;
		info.si_code = SI_QUEUE;

		//send the signal
		if(send_sig_info(SIGNR, (struct kernel_siginfo *) &info, task) < 0)
		{
			printk("error sending the signal");
		}
	}

	return (irq_handler_t) IRQ_HANDLED;
}

static long int my_ioctl(struct file *filp, unsigned cmd, unsigned long arg)
{
	if(cmd == REGISTER_UAPP)
	{
		task = get_current();
		printk("gpio_signal: user space app with PID %d is registered\n", task->pid);
	}
	return 0;
}

static int dev_release(struct inode *inode, struct file *filp)
{
	if (task != NULL)
	{
		task = NULL;
	}
	return 0;
}


static struct file_operations fops = {
		.owner = THIS_MODULE,
		.release = dev_release,
		.unlocked_ioctl = my_ioctl
	};


static int __init ModuleInit(void)
{
	printk("irq_signal - Loading Module\n");

	if(gpio_request(SignalPin, "rpi-gpio-17"))
	{
		printk("Error!: Could not assign pin %d\n", SignalPin);
		return -1;
	}

	if(gpio_direction_input(SignalPin))
	{
		printk("Error: could not set pin %d to input\n", SignalPin);
		gpio_free(SignalPin);
		return -1;
	}

	gpio_set_debounce(SignalPin, 300);

	irq_number = gpio_to_irq(SignalPin);

	if(request_irq(irq_number, (irq_handler_t) signal_handler, IRQF_TRIGGER_RISING, "my_gpio_irq_signal", NULL) != 0)
	{
		printk("Error: could not request interrupt nr %d\n", irq_number);
		gpio_free(SignalPin);
		return -1;
	}

	Major = register_chrdev(0, DEV_NAME, &fops);
	if(Major < 0)
	{
		printk("Error: could not register device number.\n");
		gpio_free(SignalPin);
		free_irq(irq_number, NULL);
		return -1;
	}

	printk("Done!. Major number: %d\n", Major);
	printk("GPIO %d mapped to IRQ Number %d\n", SignalPin, irq_number);

	return 0;
}

static void __exit ModuleExit(void)
{
	printk("irq_signal - Unloading moule\n");
	free_irq(irq_number, NULL);
	gpio_free(SignalPin);
	unregister_chrdev(Major, DEV_NAME);
	printk("Done!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
