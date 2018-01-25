HOME ?= /home/tomohiro
ARCH := arm
CROSS_COMPILE := arm-linux-gnueabihf-
KPATH := $(HOME)/rpi/linux
INCLUDE_PATH := ./include

obj-m:=myled.o gpioDriver.o

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -I $(INCLUDE_PATH) -C $(KPATH) M=$(PWD) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KPATH) M=$(PWD) clean
