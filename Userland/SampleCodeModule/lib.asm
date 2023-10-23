    global print_sys
	global get_unix_time
    global read_sys
    global draw
    global get_screen_size

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
; uint64_t draw(HexColor *figure, uint64_t dimensions, uint64_t position)
draw:
    mov rax,333
    int 80h
    ret
; uint64_t get_screen_size()
get_screen_size:
    mov rax, 337
    int 80h
    ret