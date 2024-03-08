    extern c_start

    global _start

_start:
    mov rdi, [rsp + 8]
    call c_start
    ret
