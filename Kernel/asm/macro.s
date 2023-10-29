; This file won't be directly compiled by nasm because it doesn't end in .asm :P

%macro pushall_not_rax 0
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push rsp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popall_not_rax 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsp
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro pushall 0
	push rax
	pushall_not_rax
%endmacro

%macro popall 0
	popall_not_rax
	pop rax
%endmacro
