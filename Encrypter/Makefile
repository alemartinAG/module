KDIR  := /lib/modules/$(shell uname -r)/build

obj-m += encrypter.o

all:
	make -C $(KDIR) M=$(shell pwd) modules
	$(CC) test-encrypt.c -o test

clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm test
