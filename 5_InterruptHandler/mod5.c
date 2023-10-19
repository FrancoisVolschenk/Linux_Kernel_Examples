#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h> /* This moule was written for the raspberry pi. GPIO allows us to use the pins to interact with hardware */
#include <linux/interrupt.h> /* this is used for interrupts */


MODULE_LICENSE("GPL");
MODULE_AUTHOR("FV");
MODULE_DESCRIPTION("A simple LKM to demonstrate interrupts");

//var to interrupt controller
unsigned int irq_number;

/* This function is triggered when the interrupt is called */
static irq_handler_t intrpt_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
	printk("gpio_irq was triggered and ISR was called\n");
	return (irq_handler_t) IRQ_HANDLED;
}

/* Request access to the GPIO pin, and set up an interrupt request handler */
static int __init ModuleInit(void)
{
	printk("Successfully loaded interrupt module\n");

	//set up the gpio
	if (gpio_request(17, "rpi-gpio-17"))
	{
		printk("Cannot access gpio 17\n");
		return -1;
	}

	if(gpio_direction_input(17))
	{
		printk("Could not set pin 17 to input\n");
		gpio_free(17);
		return -1;
	}

	//set up the interrupt
	irq_number = gpio_to_irq(17); // find out which interrupt handler pin 17 is linked to

	if(request_irq(irq_number, (irq_handler_t) intrpt_handler, IRQF_TRIGGER_RISING, "my_gpio_irq", NULL) != 0)
	{
		printk("Could not request interrupt number : %d\n", irq_number);
		gpio_free(17);
		return -1;
	}

	printk("GPIO 17 is mapped to IRQ %d\n", irq_number);
	return 0;
}

static void __exit ModuleExit(void)
{
	printk("Unloading interrupt module\n");
	gpio_free(17);
	free_irq(irq_number, NULL); // NULL can be replaced by device ID if a device is related
}

module_init(ModuleInit);
module_exit(ModuleExit);
