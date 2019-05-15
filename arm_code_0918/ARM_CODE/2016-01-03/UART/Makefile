
all:
	arm-linux-gcc -c test.c -o test.o -fno-builtin
	arm-linux-gcc -c basedevice.c -o basedevice.o -fno-builtin
	arm-linux-gcc -c s5p_gpio.c -o s5p_gpio.o -fno-builtin
	arm-linux-gcc -c Uart.c -o Uart.o -fno-builtin
	arm-linux-gcc -c serial_s5p.c -o serial_s5p.o -fno-builtin
	arm-linux-gcc -c s3c44b0_rtc.c -o s3c44b0_rtc.o -fno-builtin
	arm-linux-gcc -c bcd.c -o bcd.o -fno-builtin
	arm-linux-gcc -c string.c -o string.o -fno-builtin
	arm-linux-gcc -c div.S -o div.o -fno-builtin
	arm-linux-gcc -c udiv.S -o udiv.o -fno-builtin
	arm-linux-ld -Ttest.lds   *.o -o test
	arm-linux-objcopy -O binary test test.bin

clean:
	rm -rf *.o test test.bin 
