	extern move_away_from_parents_house
	extern set_process_args

	extern ncPrintHex
	extern ncNewline

	global portal_to_userland

	section .bss

execv_running_stack	resq 0x100

	section .text

; extern void portal_to_userland(int argc, char *const argv[], Process *process, EntryPoint code);
portal_to_userland:
	cli

	lea		rsp, [execv_running_stack + 0x100]
	lea		rsp, [rsp - 8 * 4]
	mov		rbp, rsp

	mov		[rbp + 8 * 0], rdi
	mov		[rbp + 8 * 1], rsi
	mov		[rbp + 8 * 2], rdx
	mov		[rbp + 8 * 3], rcx

	mov		rdi, rdx
	call	move_away_from_parents_house

	mov		rdi, [rbp + 8 * 0]
	mov		rsi, [rbp + 8 * 1]
	mov		rdx, [rbp + 8 * 2]

	call	set_process_args

	mov		rcx, [rbp + 8 * 3]

	mov		rsp, rax

	sti

	push qword  0		; SS
	push		rax		; RSP
    push qword	0x202	; RFLAGS
	push qword	0x8		; CS
	push		rcx		; Userland code

	mov	rax, 0
	mov	rbx, 0
	mov	rcx, 0
	mov	rdx, 0
	mov	rsi, 0
	mov	rdi, 0
	mov	rbp, 0
	mov	r8, 0
	mov	r9, 0
	mov	r10, 0
	mov	r11, 0
	mov	r12, 0
	mov	r13, 0
	mov	r14, 0
	mov	r15, 0

    iretq				; In Userland we trust
