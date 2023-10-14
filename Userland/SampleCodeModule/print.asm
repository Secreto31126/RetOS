    global print_sys

; int print_sys(char *str, int len)
print_sys:
    mov rax, 1
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 1
    int 80h
    ret
