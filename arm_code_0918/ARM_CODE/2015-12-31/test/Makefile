
all:
	arm-linux-gcc -c test.c -o test.o -fno-builtin
	arm-linux-ld -Ttest.lds   *.o -o test
	arm-linux-objcopy -O binary test test.bin

clean:
	rm -rf *.o test test.bin 
