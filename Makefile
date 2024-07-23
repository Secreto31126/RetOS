
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

pvs:
	./pvs.sh
	@pvs-studio-analyzer trace -o /tmp/strace_out -- make
	@pvs-studio-analyzer analyze -f /tmp/strace_out -o /tmp/project-analysis.log
	@plog-converter -a GA: -t sarif-vscode -o ./Image/report.sarif /tmp/project-analysis.log

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Global; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image collections global kernel userland all clean
