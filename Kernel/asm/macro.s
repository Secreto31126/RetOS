; This file won't be directly compiled by nasm because it doesn't end in .asm :P

; Doesn't include rax nor rsp
%macro pushall_not_rax 0
	sub		rsp, 16
	movdqu	[rsp], xmm15
	sub		rsp, 16
	movdqu	[rsp], xmm14
	sub		rsp, 16
	movdqu	[rsp], xmm13
	sub		rsp, 16
	movdqu	[rsp], xmm12
	sub		rsp, 16
	movdqu	[rsp], xmm11
	sub		rsp, 16
	movdqu	[rsp], xmm10
	sub		rsp, 16
	movdqu	[rsp], xmm9
	sub		rsp, 16
	movdqu	[rsp], xmm8
	sub		rsp, 16
	movdqu	[rsp], xmm7
	sub		rsp, 16
	movdqu	[rsp], xmm6
	sub		rsp, 16
	movdqu	[rsp], xmm5
	sub		rsp, 16
	movdqu	[rsp], xmm4
	sub		rsp, 16
	movdqu	[rsp], xmm3
	sub		rsp, 16
	movdqu	[rsp], xmm2
	sub		rsp, 16
	movdqu	[rsp], xmm1
	sub		rsp, 16
	movdqu	[rsp], xmm0

	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rbp
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
%endmacro

; Doesn't include rax nor rsp
%macro popall_not_rax 0
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop rbp
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15

	movdqu	xmm0, [rsp]
	add		rsp, 16
	movdqu	xmm1, [rsp]
	add		rsp, 16
	movdqu	xmm2, [rsp]
	add		rsp, 16
	movdqu	xmm3, [rsp]
	add		rsp, 16
	movdqu	xmm4, [rsp]
	add		rsp, 16
	movdqu	xmm5, [rsp]
	add		rsp, 16
	movdqu	xmm6, [rsp]
	add		rsp, 16
	movdqu	xmm7, [rsp]
	add		rsp, 16
	movdqu	xmm8, [rsp]
	add		rsp, 16
	movdqu	xmm9, [rsp]
	add		rsp, 16
	movdqu	xmm10, [rsp]
	add		rsp, 16
	movdqu	xmm11, [rsp]
	add		rsp, 16
	movdqu	xmm12, [rsp]
	add		rsp, 16
	movdqu	xmm13, [rsp]
	add		rsp, 16
	movdqu	xmm14, [rsp]
	add		rsp, 16
	movdqu	xmm15, [rsp]
	add		rsp, 16
%endmacro

; Doesn't include rsp
%macro pushall 0
	pushall_not_rax
	push rax
%endmacro

; Doesn't include rsp
%macro popall 0
	pop rax
	popall_not_rax
%endmacro
