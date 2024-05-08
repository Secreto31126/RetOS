include ../Makefile.inc

MODULE=0002-tomyland
SOURCES=$(wildcard *.c) $(wildcard lib/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: log $(MODULE)

log:
	@echo
	@echo "\033[0;36mMAKEFILE TOMYLAND\033[0m"
	@echo "\tSOURCES=$(SOURCES) $(SOURCES_ASM)"
	@echo

$(MODULE): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T tomyland.ld -o ../$(MODULE).bin $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
ifneq ($(DEBUG), 0)
	$(LD) $(LDFLAGS) -T tomyland.ld --oformat=elf64-x86-64 -o ../../Image/$(MODULE).elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
endif

ifneq ($(DEBUG), 0)
GCCFLAGS += -g
ASMFLAGS += -g -F dwarf
endif

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	@rm -rf *.o

.PHONY: all clean print
