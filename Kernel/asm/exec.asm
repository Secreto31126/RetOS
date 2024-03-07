	extern ncPrintHex

	global portal_to_userland

; void portal_to_userland(int (*code)(void), uint64_t rsp);
portal_to_userland:
	mov		rax, safe_return
	mov		rsp, rsi

.continue:
	push		rax		; RetOS :)
	mov			rax, rsp; Get the stack base

	push qword  0		; SS
	push		rax		; RSP
    push qword	0x202	; RFLAGS
	push qword	0x8		; CS
	push		rdi		; Userland code

    iretq				; In Userland we trust

safe_return:
	mov rdi, rax
	call ncPrintHex
.halt:
	hlt
	jmp .halt
