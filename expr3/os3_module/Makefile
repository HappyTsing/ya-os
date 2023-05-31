obj-m:=os3_module.o
CURRENT_PATH :=$(shell pwd)
VERSION_NUM :=$(shell uname -r)
LINUX_PATH :=/usr/src/linux-headers-$(VERSION_NUM)
all :
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) modules
	rm *.order *.mod.c *.o *.symvers
clean :
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) clean
