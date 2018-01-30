HOME ?= /home/tomohiro
ARCH := arm
CROSS_COMPILE := arm-linux-gnueabihf-
KPATH := $(HOME)/rpi/linux

obj-m:=myled.o
# myled-objs := myled.o myGpio.o 
clean-files := *.o *.ko *.mod.[co] *~

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KPATH) M=$(PWD) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KPATH) M=$(PWD) clean
