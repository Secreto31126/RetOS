include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: sampleCodeModule init cat wc filter loop grep sing less phylos tests sampleDataModule

sampleCodeModule:
	cd SampleCodeModule; make

init:
	cd init; make

cat:
	cd cat; make

wc:
	cd wc; make

filter:
	cd filter; make

loop:
	cd loop; make

grep:
	cd grep; make

sing:
	cd sing; make

less:
	cd less; make

phylos:
	cd phylos; make

tests:
	cd tests; make

sampleDataModule:
	whoami >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd SampleCodeModule; make clean
	cd init; make clean
	cd cat; make clean
	cd wc; make clean
	cd filter; make clean
	cd loop; make clean
	cd grep; make clean
	cd sing; make clean
	cd less; make clean
	cd phylos; make clean
	cd tests; make clean
	rm -rf *.bin


.PHONY: sampleCodeModule init cat wc filter loop grep sing less phylos tests all clean
