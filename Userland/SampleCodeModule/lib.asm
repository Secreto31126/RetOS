    global print_sys
	global get_unix_time
    global read_sys

; int print_sys(unsigned int fd, char *str, size_t len);
print_sys:
    mov rax, 1
    int 80h
    ret

; int read_sys(unsigned int fd, char *str, size_t len);
read_sys:
    mov rax, 0
    int 80h
    ret

; uint64_t get_unix_time(void);
get_unix_time:
	rdtsc
	shl rdx, 32
	or rax, rdx
	ret
