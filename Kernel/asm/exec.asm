	extern getStackBase

	global portal_to_userland

; int portal_to_userland(int argc, char *argv[], int (*code)(void), uint64_t rsp);
portal_to_userland:
	mov		rax, [rsp]
	mov		rsp, rcx
	push	0			; NULL terminated argv

.loop:
	cmp		rdi, 0
	jbe		.continue

	dec			rdi
	push qword	[rsi + rdi]
	jmp			.loop

.continue:
	push		rax		; RetOS :)
	mov			rcx, rsp

	push qword  0		; SS
	push		rcx		; RSP
    push qword	0x202	; RFLAGS
	push qword	0x8		; CS
	push		rdx		; Userland code

    iretq				; In Userland we trust
