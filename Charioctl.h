#ifndef CHARIOCTL_H
#define CHARIOCTL_H
#include <linux/ioctl.h>
#define MAJOR_NUM 80
#define IOCTL_SET_MSG _IOW(MAJOR_NUM,0,char *)
#define IOCTL_GET_MSG _IOR(MAJOR_NUM,1,char *)
typedef enum _Letter_Method
{
Up,Down,Reverse
}Letter_Method;
#endif
	
