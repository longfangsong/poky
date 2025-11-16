obj-m += mocksensor.o

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) clean

modules_install:
	make -C $(KERNEL_SRC) M=$(PWD) modules_install