include ../Makefile.inc

MODULE=0005-wc
SOURCES=$(wildcard *.c) $(wildcard lib/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=$(wildcard ../../Global/string/*.o) $(wildcard ../../Global/math/*.o) $(wildcard ../../Global/stdlib/*.o)

all: log $(MODULE)

log:
	@echo
	@echo "\033[0;36mMAKEFILE WC\033[0m"
	@echo "\tSOURCES=$(SOURCES) $(SOURCES_ASM)"
	@echo

$(MODULE): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T wc.ld -o ../$(MODULE).bin $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
ifneq ($(DEBUG), 0)
	$(LD) $(LDFLAGS) -T wc.ld --oformat=elf64-x86-64 -o ../../Image/$(MODULE).elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
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
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	@find . -name "*.o" -delete

.PHONY: all clean print
