    global handle_catch
	global setup_catch

	extern dump_reg_string

    section .text

; void handle_catch(uint64t exception);
handle_catch:
	mov		rax, [catcher]
	cmp		rax, 0
	je		.halt_on_unhandled_exception
	mov		rsp, [rbp_restore]

	mov		rsi, dump_reg_string
	jmp		rax

.halt_on_unhandled_exception:
	cli
	hlt
	jmp		.halt_on_unhandled_exception

; void setup_catch(Catch c, uint64_t rbp);
setup_catch:
	mov		[catcher], rdi
	mov		[rbp_restore], rsi
	ret

	section .data
catcher dq 0
rbp_restore dq 0
