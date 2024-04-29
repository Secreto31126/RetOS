    extern context_switch
    extern ncPrintHex
    extern ncNewline
    extern ncTab

    global scheduler
	global kernel_fork

; WARNING: RSP and RBP values are NOT preserved
scheduler:
    ; Save the return address
    pop rbp

    mov rdi, rsp
    call ncPrintHex
    call ncTab

    mov rdi, rsp
    call context_switch
    mov rsp, rax

    call ncTab
    mov rdi, rsp
    call ncPrintHex
    ; call ncNewline

    ; Restore the return address
    push rbp
    ret

; void *kernel_fork();
kernel_fork:
	mov	rax, 0xC
	int	0x80
	ret
