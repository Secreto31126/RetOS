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

	pxor	xmm0,	xmm0
	pxor	xmm1,	xmm1
	pxor	xmm2,	xmm2
	pxor	xmm3,	xmm3
	pxor	xmm4,	xmm4
	pxor	xmm5,	xmm5
	pxor	xmm6,	xmm6
	pxor	xmm7,	xmm7
	pxor	xmm8,	xmm8
	pxor	xmm9,	xmm9
	pxor	xmm10,	xmm10
	pxor	xmm11,	xmm11
	pxor	xmm12,	xmm12
	pxor	xmm13,	xmm13
	pxor	xmm14,	xmm14
	pxor	xmm15,	xmm15

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
