
OBJS:=basedevice.o  div.o  s3c44b0_rtc.o 
OBJS+=serial_s5p.o  test.o  udiv.o
OBJS+=bcd.o enable_mmu.o  s5p_gpio.o  
OBJS+=string.o Uart.o  vector.o  irq.o Timer.o  I2C.o i2c.o  gpio.o 
OBJS+=timer.o

CROSS_COMPILE=arm-linux-
CCFLAG=-fno-builtin
TARGET=test
LINKSCRIPT=test.lds

export	 CROSS_COMPILE   CCFLAG   

${TARGET}.bin:${TARGET}
	${CROSS_COMPILE}objcopy -O binary   $^   $@

${TARGET}:${OBJS}
	${CROSS_COMPILE}ld -T${LINKSCRIPT}  $^ -o  $@
	make -C  vector

%.o:%.c
	${CROSS_COMPILE}gcc	-c $^ -o $@  ${CCFLAG}
%.o:%.S
	${CROSS_COMPILE}gcc -c $^ -o $@

clean:
	rm -rf ${OBJS}  ${TARGET}.bin  ${TARGET}
	make clean -C vector
