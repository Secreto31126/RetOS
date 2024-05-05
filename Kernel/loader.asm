	extern initializeKernelBinary
	extern init

	global loader

	section .text

loader:
	call	initializeKernelBinary

	; Start using the init stack
	mov		rsp, rax
	sti

	; Fork the first process
	mov		rax, 0xF
	int		80h

	mov		rdi, rax
	call 	init

.hang:
	cli
	hlt
	jmp	.hang
