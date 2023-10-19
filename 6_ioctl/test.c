#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "ioctl_test.h"


int main()
{
	int answer;
	struct mystruct test = {68, "Joe"};

	int dev = open("/dev/mydevice", O_RDONLY);
	if(dev == -1)
	{
		printf("Could not open /dev/mydevice\n");
		return -1;

	}
	printf("Opening was successful\n");

	ioctl(dev, RD_VALUE, &answer);
	printf("The answer is %d\n", answer);

	answer = 123;
	ioctl(dev, WR_VALUE, &answer);
	answer = 52;
	ioctl(dev, RD_VALUE, &answer);
	printf("The updated answer is %d\n", answer);

	ioctl(dev, GREETER, &test);

	close(dev);

	return 0;
}
