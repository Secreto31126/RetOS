    global print_sys
	global get_unix_time
    global read_sys

; int print_sys(char *str, int len)
print_sys:
    mov rax, 1
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 1 ; only prints to stdout
    int 80h
    ret

    ; int read_sys(char *str, int len)
read_sys:
    mov rax, 0
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 0 ; only reads from stdin
    int 80h
    ret

; uint64_t get_unix_time(void);
get_unix_time:
	rdtsc
	shl rdx, 32
	or rax, rdx
	ret
