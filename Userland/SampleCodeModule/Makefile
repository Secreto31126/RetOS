include ../Makefile.inc

MODULE=0000-sampleCodeModule
SOURCES=$(wildcard [^_]*.c) $(wildcard nstdlib/*.c) $(wildcard window/*.c) $(wildcard shell/*.c) $(wildcard shell/commandHandler/*.c) $(wildcard snake/*.c) $(wildcard snake/drawings/*.c) $(wildcard moduleSelector/*.c) $(wildcard piano/*.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=_loader.c

LOADEROBJECT=$(LOADERSRC:.c=.o)
STATICLIBS=$(wildcard ../../Global/stdlib/*.o) $(wildcard ../../Global/string/*.o) $(wildcard ../../Global/stdmem/*.o)

all: log $(MODULE)

log:
	@echo
	@echo "\033[0;36mMAKEFILE USERSPACE\033[0m"
	@echo "\tSOURCES=$(SOURCES) $(SOURCES_ASM)"
	@echo

$(MODULE): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE).bin $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
ifneq ($(DEBUG), 0)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld --oformat=elf64-x86-64 -o ../../Image/$(MODULE).elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
endif

ifneq ($(DEBUG), 0)
GCCFLAGS += -g
ASMFLAGS += -g -F dwarf
endif

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -I../../Global/include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(GCC) $(GCCFLAGS) -c $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	@find . -name "*.o" -delete

.PHONY: all clean print
