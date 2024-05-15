	extern context_switch
	extern yield_robin
	extern set_exit_code
	extern get_pid
	extern kill_process

	global scheduler
	global yield
	global exit
	global swap_stacks

	section .bss

scheduler_running_stack	resb 0x800
exit_running_stack		resb 0x800

	section .text

; WARNING: RSP value is NOT preserved
scheduler:
	; Save the return address
	xchg	rax, [rsp]
	mov		[scheduler_running_stack + 0x800 - 8], rax
	pop		rax

	mov		rdi, rsp
	lea		rsp, [scheduler_running_stack + 0x800 - 8]

	call	context_switch
	mov		rsp, rax

	mov		rax, [scheduler_running_stack + 0x800 - 8]
	push	rax

	ret

; void yield();
yield:
	call	yield_robin
	int		0x20
	ret

; void exit(int status);
exit:
	lea		rsp, [exit_running_stack + 0x800]
	; Any last words?
	call	set_exit_code

.kill:
	call	get_pid
	mov		rdi, rax
	call	kill_process

	; Keep killing until it's dead dead or the scheduler stops you
	cmp		rax, 0
	jne		.kill

	; Let the scheduler pick your soul and die happily
	call	yield

	; Just in case
.loop:
	sti
	hlt
	jmp		.loop

; void swap_stacks(void *current, void *stash, size_t size);
swap_stacks:
	cmp		rdi, rsi
	je		.end

.loop:
	cmp		rdx, 8
	jb		.last_bits

	sub		rdx, 8

	; mov [rdi + rdx], [rsi + rdx]
	mov		rcx, [rdi + rdx]
	xchg	rcx, [rsi + rdx]
	mov		[rdi + rdx], rcx
 
	cmp		rdx, 0
	je		.end
	jmp		.loop

.last_bits:
	cmp		rdx, 0
	jb		.end

	dec		rdx

	mov		cl, [rdi + rdx]
	xchg	cl, [rsi + rdx]
	mov		[rdi + rdx], cl

	jmp		.last_bits

.end:
	ret
