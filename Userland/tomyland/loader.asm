	extern main

	global _start

_start:
	mov		rdi, [rsp]
	lea		rsi, [rsp + 8]
	call	main

	; In the future, an exit syscall will go here

	ret
