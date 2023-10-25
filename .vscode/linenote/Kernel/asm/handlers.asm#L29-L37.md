```asm
%macro slave_pic_handler 1
	pushall
	mov		rdi, %1
	call	pic_manager
	mov     al, 0x20
	out     0xA0, al
	mov     al, 0x20
	out     0x20, al
	popall
	iretq
%endmacro
```
