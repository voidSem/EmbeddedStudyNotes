
obj-m=test.o

#obj-y  直接编译进内核
#obj-m  编译成模块的方式插入内核
#test.o 内核用test.o编译成一个内核驱动模块

#指定内核源码树的路径,用内核源码树下的Makefile进行编译

KERN=/usr/src/kernels/2.6.32-573.el6.x86_64


#写内核编译流程

#make modules 的意思是编译成一个模块
#-C 指定内核目录
#M  编译成的模块所存放的目录
#`  可执行符号
#pwd  将编译成的模块存在当前的目录下

#xxx.ko  以ko为结尾的文件叫内核驱动模块

all:
	make -C ${KERN} M=`pwd`   modules
clean:
	make -C ${KERN} M=`pwd`   modules clean
	rm -rf Module*

