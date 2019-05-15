
obj-m=mydevice1.o

KERN=/disk/A9/linux-3.5


all:
	make -C ${KERN} M=`pwd` modules
clean:
	make -C ${KERN} M=`pwd` modules clean
	rm -rf Module*
