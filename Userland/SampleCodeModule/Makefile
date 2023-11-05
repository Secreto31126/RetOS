include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c) $(wildcard nstdlib/*.c) $(wildcard window/*.c) $(wildcard shell/*.c) $(wildcard shell/commandHandler/*.c) $(wildcard snake/*.c) $(wildcard snake/drawings/*.c) $(wildcard moduleSelector/*.c) $(wildcard piano/*.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=_loader.c

LOADEROBJECT=$(LOADERSRC:.c=.o)
STATICLIBS=

all: log $(MODULE)

log:
	@echo
	@echo "\033[0;36mMAKEFILE USERSPACE\033[0m"
	@echo "\tSOURCES=$(SOURCES) $(SOURCES_ASM)"
	@echo

$(MODULE): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(GCC) $(GCCFLAGS) -c $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf *.o

.PHONY: all clean print
