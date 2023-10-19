#ifndef IOCTL_TEST_H
#define IOCTL_TEST_H

struct mystruct
{
	int repeat;
	char name[64];
};

#define WR_VALUE _IOW('a', 'b', int32_t *) //write from user space to kernel space
#define RD_VALUE _IOR('a', 'b', int32_t *) //read from kernel space to user space
#define GREETER _IOW('a', 'c', struct mystruct *)


#endif
