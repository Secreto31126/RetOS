	extern execv
	extern initializeKernelBinary

	global loader

	section .data
userland	db "module", 0
tomyland	db "tomyland", 0

	section .text

loader:
	call	initializeKernelBinary	; Set up the kernel binary
	; mov		rdi, userland
	mov		rdi, tomyland
	mov		rsi, 0
	call 	execv
.hang:
	cli
	hlt
	jmp	.hang
