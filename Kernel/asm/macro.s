; This file won't be directly compiled by nasm because it doesn't end in .asm :P

%macro pushall_not_rax 0
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rsp
	push rbp
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
%endmacro

%macro popall_not_rax 0
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop rbp
	pop rsp
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
%endmacro

%macro pushall 0
	pushall_not_rax
	push rax
%endmacro

%macro popall 0
	pop rax
	popall_not_rax
%endmacro
