	extern initializeKernelBinary
	extern idle
	extern move_away_from_parents_house

	global loader
	global kfork
	global daemon_init

	section .bss

daemon_running_stack	resb 0x400

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

; void daemon_init(void (*daemon)(void));
daemon_init:
	lea		rsp, [daemon_running_stack + 0x400]

	push	rdi
	xor		rdi, rdi

	cli
	call	move_away_from_parents_house
	sti

	pop		rdi
	mov		rsp, rax

	push	rdi
	ret
