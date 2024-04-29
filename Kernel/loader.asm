	extern initializeKernelBinary
	extern start_userland

	global loader

	section .data
userland	db "module", 0
tomyland	db "tomyland", 0

hello		db "Hello", 0
world		db "World", 0
argv		dq hello, world, 0

	section .text

loader:
	call	initializeKernelBinary	; Set up the kernel binary
	call 	start_userland
	; mov		rdi, userland
	; mov		rdi, tomyland
	; mov		rsi, argv
	; call 	execv
.hang:
	; cli
	hlt
	jmp	.hang
