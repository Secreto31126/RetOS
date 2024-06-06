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
    global _exit
    global kill
    global getpid
    global execv
    global fork
    global sched_yield
    global waitpid
    global sleep
    global pipe
    global close
    global dup2
    global usleep
    global pselect
    global flush
    global getpriority
    global setpriority
    global sem_open
    global sem_close
    global sem_unlink
    global sem_post
    global sem_wait

; size_t read(int fd, void *buf, size_t count);
read:
    mov rax, 0
    int 80h
    ret

; size_t write(int fd, void *buf, size_t count);
write:
    mov rax, 1
    int 80h
    ret

; uint64_t draw(HexColor *figure, uint64_t dimensions, uint64_t position);
draw:
    mov rax, 2
    int 80h
    ret

; uint64_t malloc(uint32_t size);
malloc:
    mov rax, 3
    int 80h
    ret

; void free(uint64_t ptr);
free:
    mov rax, 4
    int 80h
    ret

; uint64_t get_unix_time(void);
get_unix_time:
	mov rax, 5
    int 80h
	ret

; uint64_t get_screen_size();
get_screen_size:
    mov rax, 6
    int 80h
    ret

; void beep(uint32_t frequency);
beep:
    mov rax, 7
    int 80h
    ret

; uint64_t get_tick();
get_tick:
    mov rax, 8
    int 80h
    ret

; void halt_user();
halt_user:
    mov rax, 0xA
    int 80h
    ret

; void exit();
_exit:
    mov rax, 0xB
    int 80h
    ret

; int kill(pid_t pid, int sig);
kill:
    mov rax, 0xC
    int 80h
    ret

; int get_pid();
getpid:
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
sched_yield:
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

; int usleep(unsigned int usec);
usleep:
    mov rax, 0x16
    int 80h
    ret

; int pselect(int nfds, const int *fds, int *ready);
pselect:
    mov rax, 0x17
    int 80h
    ret

; int flush(int fd);
flush:
    mov rax, 0x18
    int 80h
    ret

; int getpriority(int which, unsigned int who);
getpriority:
    mov rax, 0x19
    int 80h
    ret

; int setpriority(int which, unsigned int who, int prio);
setpriority:
    mov rax, 0x1A
    int 80h
    ret

; sem_t *sem_open(const char *name, unsigned int value);
sem_open:
    mov rax, 0x1B
    int 80h
    ret

; int sem_close(sem_t *sem)
sem_close:
    mov rax, 0x1C
    int 80h
    ret

; int sem_unlink(const char *name)
sem_unlink:
    mov rax, 0x1D
    int 80h
    ret

; int sem_post(sem_t *sem)
sem_post:
    mov rax, 0x1E
    int 80h
    ret

; int sem_wait(sem_t *sem)
sem_wait:
    mov rax, 0x1F
    int 80h
    ret
