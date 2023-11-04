	extern exception_manager
	extern pic_manager
	extern syscall_manager

	extern dump_regs
	extern dump_regs_include_rip

	extern getStackBase
	extern main

	global zero_division_exception_handler
	global invalid_opcode_exception_handler
	global sus_exception_handler

	global tick_handler
	global keyboard_handler
	global cascade_pic
	global even_ports_handler
	global odd_ports_handler
	global usb_handler

	global syscall_handler

; Learnt this trick from pure64.asm
%include "macro.s"

pain_manager:
	hlt
	jmp		pain_manager

%macro exception_handler 1
	call	dump_regs
	mov		rdi, [rsp]
	call	dump_regs_include_rip

	mov		rdi, %1
	call	exception_manager

	mov		qword [rsp], pain_manager

	call 	getStackBase
	mov		qword	[rax], pain_manager
	sub		rax, 8
	mov 	[rsp + 8 * 3], rax

	iretq
%endmacro

%macro master_pic_handler 1
	pushall
	mov		rdi, %1
	call	pic_manager
	mov		al, 0x20
	out		0x20, al
	popall
	iretq
%endmacro

%macro slave_pic_handler 1
	pushall
	mov		rdi, %1
	call	pic_manager
	mov		al, 0x20
	out		0xA0, al
	mov		al, 0x20
	out		0x20, al
	popall
	iretq
%endmacro

; void zero_division_handler(void);
zero_division_exception_handler:
	exception_handler 0

; void invalid_opcode_exception_handler(void);
invalid_opcode_exception_handler:
	exception_handler 6

sus_exception_handler:
	exception_handler -1

; void tick_handler(void);
tick_handler:
	master_pic_handler 0

; void keyboard_handler(void);
keyboard_handler:
	master_pic_handler 1

; void cascade_pic(void);
cascade_pic:
	master_pic_handler 2

; void even_ports_handler(void);
even_ports_handler:
	master_pic_handler 3

; void odd_ports_handler(void);
odd_ports_handler:
	master_pic_handler 4

; void usb_handler(void);
usb_handler:
	master_pic_handler 5

; uint64_t syscall_handler(void);
syscall_handler:
	pushall_not_rax
	mov		rcx, rax
	call	syscall_manager
	popall_not_rax
	iretq
