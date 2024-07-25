	extern initializeKernelBinary
	extern idle

	global loader
	global kfork

	section .text

loader:
	call	initializeKernelBinary

	; Start using the idle stack
	mov		rsp, rax

	; Fork the first process
	call	kfork

	mov		rdi, rax
	call 	idle

.hang:
	cli
	hlt
	jmp	.hang

kfork:
	mov		rax, 0xF
	int		80h
	ret
