	extern syscall_manager

	global cpuVendor
	global input_dword
	global output_dword
	global input_byte
	global output_byte
	global syscall_handler

	section .text

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

; uint32_t input(uint16_t port);
input_dword:
	mov edx, edi
	in	eax, dx
	ret

; void output(uint16_t port, uint32_t value);
output_dword:
	mov edx, edi
	mov eax, esi
	out dx, eax
	ret

; uint8_t input(uint16_t port);
input_byte:
	mov edx, edi
	in	al, dx
	ret

; void output(uint16_t port, uint8_t value);
output_byte:
	mov edx, edi
	mov eax, esi
	out dx, al
	ret

; uint64_t syscall_handler(void);
syscall_handler:
	push	rcx
	mov		rcx, rax
	call	syscall_manager
	pop		rcx
	iretq
