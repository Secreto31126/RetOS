	extern main

	global _start

_start:
	mov		rdi, [rsp]
	lea		rsi, [rsp + 8]

	call	main

	mov		rdi, rax ; Lol, Kernel doesn't care about your "status code"
	mov		rax, 0xB
	int		80h
