	extern initializeKernelBinary
	extern idle

	global loader

	section .text

loader:
	call	initializeKernelBinary

	; Start using the idle stack
	mov		rsp, rax
	sti

	; Fork the first process
	mov		rax, 0xF
	int		80h

	mov		rdi, rax
	call 	idle

.hang:
	cli
	hlt
	jmp	.hang
