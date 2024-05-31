	extern memcpy
	extern dump_regs_hex_magician

	global cpuVendor
	global input_dword
	global output_dword
	global input_word
	global output_word
	global input_byte
	global output_byte
	global set_interrupt_flag
	global unset_interrupt_flag
	global halt_once
	global exchange
	global dump_regs
	global dump_reg_string

	section .text

%include "./macro.s"

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

; uint32_t input_dword(uint16_t port);
input_dword:
	mov edx, edi
	in	eax, dx
	ret

; void output_dword(uint16_t port, uint32_t value);
output_dword:
	mov edx, edi
	mov eax, esi
	out dx, eax
	ret

; uint16_t input_word(uint16_t port);
input_word:
	mov edx, edi
	in	ax, dx
	ret

; void output_word(uint16_t port, uint16_t value);
output_word:
	mov edx, edi
	mov eax, esi
	out dx, ax
	ret

; uint8_t input_byte(uint16_t port);
input_byte:
	mov edx, edi
	in	al, dx
	ret

; void output_byte(uint16_t port, uint8_t value);
output_byte:
	mov edx, edi
	mov eax, esi
	out dx, al
	ret

; void set_interrupt_flag(void);
set_interrupt_flag:
	sti
	ret

; void unset_interrupt_flag(void);
unset_interrupt_flag:
	cli
	ret

; void halt_once(void);
halt_once:
	hlt
	ret

; uint64_t exchange(uint64_t *src, uint64_t value);
exchange:
	mov		rax, rsi
	xchg	rax, [rdi]
	ret

%macro save_hex_in_string 0
	lea	rdi, [dump_reg_string + rcx]
	mov rsi, rax

	push rax
	push rcx
	; void dump_regs_hex_magician(char *s, uint8_t r);
	call dump_regs_hex_magician
	pop rcx
	pop rax

	add rcx, 2
%endmacro

; @source ./Bootloader/Pure64/src/pure64.asm
; void dump_regs(void);
dump_regs:
	pushall
	push rsp

	mov rbp, rsp
	mov rbx, 0		; stage
	mov rcx, 0		; length

.dump_regs_again:
	lea	rdi, [dump_reg_string + rcx]

	mov rsi, dump_reg_string00
	mov rax, rbx
	shl rax, 2				; rax *= 2^2
	add rsi, rax

	mov rdx, 4 ; each string is 4 bytes

	; void *memcpy(void *dest, const void *src, uint64_t length);
	push rcx
	call memcpy
	pop rcx

	add rcx, 4

	mov rax, [rbp]
	add rbp, 8

; rax
	ror rax, 56
	save_hex_in_string
	rol rax, 8
	save_hex_in_string
	rol rax, 8
	save_hex_in_string
	rol rax, 8
	save_hex_in_string
	rol rax, 32
; eax
	ror rax, 24
	save_hex_in_string
	rol rax, 8
	save_hex_in_string
	rol rax, 16
; ah
	ror rax, 8
	save_hex_in_string
; al
	rol rax, 8
	save_hex_in_string

	mov byte [dump_reg_string + rcx], 0x0A
	inc rcx

	inc rbx
	cmp rbx, 0x10
	jb .dump_regs_again

	mov byte [dump_reg_string + rcx], 0

	pop rsp
	popall
	ret

dump_reg_string00: db 'RSP:'
dump_reg_string01: db 'RAX:'
dump_reg_string02: db 'RBX:'
dump_reg_string03: db 'RCX:'
dump_reg_string04: db 'RDX:'
dump_reg_string05: db 'RSI:'
dump_reg_string06: db 'RDI:'
dump_reg_string07: db 'RBP:'
dump_reg_string08: db ' R8:'
dump_reg_string09: db ' R9:'
dump_reg_string0A: db 'R10:'
dump_reg_string0B: db 'R11:'
dump_reg_string0C: db 'R12:'
dump_reg_string0D: db 'R13:'
dump_reg_string0E: db 'R14:'
dump_reg_string0F: db 'R15:'

	section .bss
; extern char dump_reg_string[360];
dump_reg_string: resb 360
