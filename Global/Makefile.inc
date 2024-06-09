GCC=gcc
AR=ar
ASM=nasm

DEBUG=0
EGCCFLAGS=
GCCFLAGS=-m64 -fno-exceptions -fno-asynchronous-unwind-tables -mno-mmx -mno-sse -mno-sse2 -fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc -mno-red-zone -Wall -ffreestanding -nostdlib -fno-common -std=c99 -fdiagnostics-color=always $(EGCCFLAGS)
ARFLAGS=rvs
ASMFLAGS=-felf64
