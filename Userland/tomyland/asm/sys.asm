    global read
    global write
    global draw
    global malloc
    global free
	global get_unix_time
    global get_screen_size
    global beep
    global get_tick
    global get_dump
    global halt_user

; int read(unsigned int fd, char *str, size_t len);
read:
    mov rax, 0
    int 80h
    ret

; int write(unsigned int fd, char *str, size_t len);
write:
    mov rax, 1
    int 80h
    ret

; uint64_t draw(HexColor *figure, uint64_t dimensions, uint64_t position)
draw:
    mov rax, 2
    int 80h
    ret

; uint64_t malloc(uint64_t size)
malloc:
    mov rax, 3
    int 80h
    ret

; void free(uint64_t ptr)
free:
    mov rax, 4
    int 80h
    ret

; uint64_t get_unix_time(void);
get_unix_time:
	mov rax, 5
    int 80h
	ret

; uint64_t get_screen_size()
get_screen_size:
    mov rax, 6
    int 80h
    ret

; void beep(uint32_t frequency)
beep:
    mov rax, 7
    int 80h
    ret

; uint64_t get_tick()
get_tick:
    mov rax, 8
    int 80h
    ret

; void halt_user();
halt_user:
    mov rax, 0xA
    int 80h
    ret
