KDIR  := /lib/modules/$(shell uname -r)/build

obj-m += ebbchar.o

all:
	make -C $(KDIR) M=$(shell pwd) modules
	$(CC) testebbchar.c -o test

clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm test
