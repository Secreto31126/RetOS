	global portal_to_userland

; void portal_to_userland(int (*code)(void), uint64_t rsp);
portal_to_userland:
	push qword  0		; SS
	push		rdi		; RSP
    push qword	0x202	; RFLAGS
	push qword	0x8		; CS
	push		rdi		; Userland code

    iretq				; In Userland we trust
