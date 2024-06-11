	extern exception_manager
	extern pic_manager
	extern syscall_manager

	extern dump_regs
	extern dump_regs_include_rip

	; extern execv
	extern scheduler

	global zero_division_exception_handler
	global invalid_opcode_exception_handler
	global sus_exception_handler

	global tick_handler
	global keyboard_handler
	global cascade_pic
	global even_ports_handler
	global odd_ports_handler
	global usb_handler

	global ethernet_handler

	global syscall_handler

	section .data
userland	db "module", 0

	section .text
; Learnt this trick from pure64.asm
%include "macro.s"

%macro full_dump_interruption 0
	call	dump_regs
	push	rdi
	mov		rdi, [rsp + 8]
	pushall
	call	dump_regs_include_rip
	popall
	pop		rdi
%endmacro

%macro exception_handler 1
	full_dump_interruption

	mov		rdi, %1
	call	exception_manager

	mov		rdi, userland
	mov		rsi, 0
	; call 	execv
.hang:
	cli
	hlt
	jmp	.hang
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
	pushall

	mov		rdi, 0
	call	pic_manager

	call	scheduler

	mov		al, 0x20
	out		0x20, al

	popall

	iretq

; void keyboard_handler(void);
keyboard_handler:
	full_dump_interruption
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

ethernet_handler:
	slave_pic_handler 11

; uint64_t syscall_handler(void);
syscall_handler:
	pushall_not_rax
	; Set rax to 0 if a fork is requested
	push	qword 0

	mov		rcx, rax
	mov		r8, rsp

	call	syscall_manager

	; Ignore rax value
	add		rsp, 8
	popall_not_rax

	iretq
