#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h> /* This moule was written for the raspberry pi, gpio allows us to use the pins to control physical hardware */
#include <linux/jiffies.h> /* Jiffies are used to calculate time differences */
#include <linux/timer.h> /* This module is used to manage timers */


MODULE_LICENSE("GPL");
MODULE_AUTHOR("FV");
MODULE_DESCRIPTION("A simple LKM to demonstrate timer events");

static struct timer_list my_timer;

/* This method is triggered when the timer lapses */
void timer_callback(struct timer_list * data)
{
	printk("Timer callbak - LED Off");
	gpio_set_value(4, 0); /* This turns an LED off that was hooked up to the pi */
}

/* This method requests access to the GPIO pin to light up an LED. It also sets up the timer and points it to the correct function to trigger */
static int __init ModuleInit(void)
{
	printk("Successfully loaded the timer module\n");
	if(gpio_request(4, "rpi-gpio-4"))
	{
		printk("Could not allocate GPIO 4\n");
		return -1;
	}

	if(gpio_direction_output(4, 0))
	{
		printk("Could not set pin 4 to output.\n");
		gpio_free(4);
		return -1;
	}
	//Turn LED on
	gpio_set_value(4, 1);
	printk("LED On");
	timer_setup(&my_timer, timer_callback, 0);
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));

	return 0;
}

static void __exit ModuleExit(void)
{
	printk("Unloading timer module!\n");
	gpio_free(4);
	del_timer(&my_timer);
}


module_init(ModuleInit);
module_exit(ModuleExit);
