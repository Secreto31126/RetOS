include Makefile.inc

KERNEL=kernel.bin
KERNELSRC=kernel.c
SOURCES=$(KERNELSRC) $(wildcard exec/*.c) $(wildcard lib/*.c) $(wildcard ticks/*.c) $(wildcard console/*.c) $(wildcard audio/*.c) $(wildcard video/*.c) $(wildcard images/*.c) $(wildcard modules/*.c) $(wildcard localization/*.c) $(wildcard stdin/*.c) $(wildcard stderr/*.c) $(wildcard stdkey/*.c) $(wildcard interruptions/*.c) $(wildcard header/*.c) $(wildcard memory/*.c) # $(wildcard folder_name/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: log $(KERNEL)

log:
	@echo
	@echo "\033[0;36mMAKEFILE KERNEL\033[0m"
	@echo "\tSOURCES=$(SOURCES) $(SOURCES_ASM)"
	@echo

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -I../Global/include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) -i ./asm/ $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	@find . -name **.o -delete -or -name **.bin -delete

.PHONY: all clean
