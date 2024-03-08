	extern c_start

	global _start

_start:
	lea		rdi, [rsp + 8]
	call	c_start
	ret
