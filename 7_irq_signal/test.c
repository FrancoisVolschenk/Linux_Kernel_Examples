#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

#define SIGTX 44
# define REGISTER_UAPP _IO('R', 'g')

void signalhandler(int sig)
{
	printf("Signal recieved!\n");
}

int main()
{
	signal(SIGTX, signalhandler);

	printf("PID: %d\n", getpid());

	int fd = open("/dev/irq_signal", O_RDONLY);
	if(fd < 0)
	{
		printf("Could not open device file\n");
		return -1;
	}

	//register this process with the driver
	if(ioctl(fd, REGISTER_UAPP, NULL))
	{
		printf("error registering app\n");
		close(fd);
		return -1;
	}

	printf("waiting for signal....\n");

	while(1)
	{
		sleep(1);
	}

	return 0;
}
