
all:  bootloader global kernel userland image

bootloader:
	cd Bootloader; make all

global:
	cd Global; make all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Global; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image collections global kernel userland all clean
