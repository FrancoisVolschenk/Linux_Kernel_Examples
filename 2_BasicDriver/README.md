# About
This module demonstrates basic driver functionality. It emulates a driver for a virtual device.

# Usage
- build the module `make`
- load the module into the kernel `sudo insmod ./mod2.ko`
- print the kernel logs to get the assigned Major number `sudo journalctl --since "1 min ago"| grep "kernel"`
- If a positive major number is printed, that means the driver has been registered. Lets say the number is 238
- create a virtual device that uses the driver. `sudo mknod /dev/VirtDevice c 238 0` (replace 238 with the correct number)
- Try reading from the device `cat /dev/VirtDevice`
- look at the kernel logs again `sudo journalctl --since "1 min ago"| grep "kernel"`
- unload the module `sudo rmmod mod2`
