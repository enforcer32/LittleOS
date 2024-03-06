[BITS 32]

global _start
extern KMain

CODE_SEG equ 0x08 ; Selector 0x08 will be our code segment offset
DATA_SEG equ 0x10 ; Selector 0x10 will be our data segment offset

_start:
	; set data segment
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov ebp, 0x00200000
	mov esp, ebp

	; Enable A20 Line to access all memory
	in al, 0x92
	or al, 2
	out 0x92, al

	; Kernel
	call KMain

	jmp $

times 512-($-$$) db 0
