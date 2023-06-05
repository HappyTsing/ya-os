obj-m :=os4_charMsgModule.o
os4_charMsgModule-objs :=DriverFileOperation.o charMsgModule.o

kernel_path=/lib/modules/$(shell uname -r)/build
PWD :=$(shell pwd)

all:
	make -C $(kernel_path) M=$(PWD) modules
	rm *.order *.mod.c *.o *.symvers
clean:
	make -C $(kernel_path) M=$(PWD) clean