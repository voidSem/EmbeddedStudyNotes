
all:
	arm-linux-gcc -c vector.S -o vector.o 
	arm-linux-ld -Ttext=0xFFFF0000 vector.o -o vector
	arm-linux-objcopy -O binary vector vector.bin
clean:
	rm -rf  vector.o vector.bin  vector
