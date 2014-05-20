obj-m :=chardev.o
CURRENT_PATH:=$(shell pwd)
LINUX_KERNEL:=$(shell uname -r)
LINXU_KERNEL_PATH:=/lib/modules/$(LINUX_KERNEL)/build
all:
	make -C $(LINXU_KERNEL_PATH) M=$(CURRENT_PATH) modules
clean:
	make -C $(LINXU_KERNEL_PATH) M=$(CURRENT_PATH) clean
install:
	insmod chardev.ko
uninstall:
	rmmod  chardev
		
