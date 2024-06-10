GCC=gcc
GPP=g++
LD=ld
AR=ar
ASM=nasm

DEBUG=0
EGCCFLAGS=
GCCFLAGS=-m64 -fno-exceptions -std=c99 -Wall -ffreestanding -nostdlib -fno-common -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc -fno-pic -fdiagnostics-color=always $(EGCCFLAGS)
ARFLAGS=rvs
ASMFLAGS=-felf64
ELDFLAGS=
LDFLAGS=--warn-common -z max-page-size=0x1000 $(ELDFLAGS)
