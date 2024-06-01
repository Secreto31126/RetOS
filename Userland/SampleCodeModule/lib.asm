    global read_sys
    global print_sys
    global draw
    global malloc
    global free
	global get_unix_time
    global get_screen_size
    global beep
    global get_tick
    global get_dump
    global halt_user
    global exit
    global get_pid
    global execv
    global fork
    global yield
    global waitpid
    global sleep
    global pipe
    global close
    global dup2
    global pselect
    global kill

; int read_sys(unsigned int fd, char *str, size_t len);
read_sys:
    mov rax, 0
    int 80h
    ret

; int print_sys(unsigned int fd, char *str, size_t len);
print_sys:
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

; void beep(uint64_t frequency)
beep:
    mov rax, 7
    int 80h
    ret

; uint64_t get_tick()
get_tick:
    mov rax, 8
    int 80h
    ret

get_dump:
    mov rax, 9
    int 80h,
    ret

; void halt_user();
halt_user:
    mov rax, 0xA
    int 80h
    ret

exit:
    mov rax, 0xB
    int 80h
    ret

; int get_pid();
get_pid:
    mov rax, 0xD
    int 80h
    ret

; int execv(char *pathname, char *argv[]);
execv:
    mov rax, 0xE
    int 80h
    ret

; int fork();
fork:
    mov rax, 0xF
    int 80h
    ret

; void yield();
yield:
    mov rax, 0x10
    int 80h
    ret

; void waitpid();
waitpid:
    mov rax, 0x11
    int 80h
    ret

; void sleep(size_t ticks);
sleep:
    mov rax, 0x12
    int 80h
    ret

; int pipe(int pipefd[2]);
pipe:
    mov rax, 0x13
    int 80h
    ret

; int close(int fd);
close:
    mov rax, 0x14
    int 80h
    ret

; int dup2(int oldfd, int newfd);
dup2:
    mov rax, 0x15
    int 80h
    ret

; int pselect(int nfds, const int *fds, int *ready);
pselect:
    mov rax, 0x16
    int 80h
    ret

; int kill(pid_t pid, int sig);
kill:
    mov rax, 0xC
    int 80h
    ret
