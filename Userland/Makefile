include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: sampleCodeModule tomyland sampleDataModule

sampleCodeModule:
	cd SampleCodeModule; make

tomyland:
	cd tomyland; make

sampleDataModule:
	whoami >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd SampleCodeModule; make clean
	cd tomyland; make clean
	rm -rf *.bin


.PHONY: sampleCodeModule tomyland all clean
