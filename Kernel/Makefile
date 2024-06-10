include Makefile.inc

KERNEL=kernel
KERNELSRC=kernel.c
SOURCES=$(KERNELSRC) $(wildcard mman/*.c) $(wildcard proc/*.c) $(wildcard exec/*.c) $(wildcard pipes/*.c) $(wildcard files/*.c) $(wildcard lib/*.c) $(wildcard ticks/*.c) $(wildcard console/*.c) $(wildcard audio/*.c) $(wildcard video/*.c) $(wildcard images/*.c) $(wildcard modules/*.c) $(wildcard localization/*.c) $(wildcard stdin/*.c) $(wildcard stdout/*.c) $(wildcard stderr/*.c) $(wildcard stdkey/*.c) $(wildcard interruptions/*.c) $(wildcard semaphore/*.c) # $(wildcard folder_name/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=$(wildcard ../Global/memory/*.o) $(wildcard ../Global/string/*.o) $(wildcard ../Global/stdlib/*.o)

all: log $(KERNEL)

log:
	@echo
	@echo "\033[0;36mMAKEFILE KERNEL\033[0m"
	@echo "\tSOURCES=$(SOURCES) $(SOURCES_ASM)"
	@echo

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL).bin $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
ifneq ($(DEBUG), 0)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o ../Image/$(KERNEL).elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
endif

ifneq ($(DEBUG), 0)
GCCFLAGS += -g
ASMFLAGS += -g -F dwarf
endif

ifneq ($(BUDDY), 0)
GCCFLAGS += -DBUDDY_SYSTEM
endif

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -I../Global/include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) -i ./asm/ $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	@find . -name "**.o" -delete -or -name "**.bin" -delete

.PHONY: all clean
