	GLOBAL cpuVendor
	GLOBAL inb
	GLOBAL outb

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

; uint32_t inb(uint16_t port);
inb:
	inb	di, eax
	ret

; void outb(uint16_t port, uint32_t value);
outb:
	outb	di, esi
	ret
