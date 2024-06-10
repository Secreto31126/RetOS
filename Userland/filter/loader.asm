	extern main

	global _start

_start:
	mov		rdi, [rsp]
	lea		rsi, [rsp + 8]

	call	main

	mov		rdi, rax
	mov		rax, 0xB
	int		80h
