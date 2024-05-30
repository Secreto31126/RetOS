include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: sampleCodeModule tomyland cat wc filter sampleDataModule

sampleCodeModule:
	cd SampleCodeModule; make

tomyland:
	cd tomyland; make

cat:
	cd cat; make

wc:
	cd wc; make

filter:
	cd filter; make

sampleDataModule:
	whoami >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd SampleCodeModule; make clean
	cd tomyland; make clean
	cd cat; make clean
	cd wc; make clean
	cd filter; make clean
	rm -rf *.bin


.PHONY: sampleCodeModule tomyland cat wc filter all clean
