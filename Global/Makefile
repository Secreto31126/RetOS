include Makefile.inc

SOURCES=$(wildcard strings/*.c)# $(wildcard folder_name/*.c)
SOURCES_ASM=# $(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: log $(OBJECTS) $(OBJECTS_ASM)

log:
	@echo
	@echo "\033[0;36mMAKEFILE GLOBAL\033[0m"
	@echo "\tSOURCES=$(SOURCES) $(SOURCES_ASM)"
	@echo

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) -i ./asm/ $< -o $@

clean:
	@find . -name "**.o" -delete -or -name "**.bin" -delete

.PHONY: all clean
