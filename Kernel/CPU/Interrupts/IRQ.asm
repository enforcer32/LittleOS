section .text

extern IRQHandler

global IRQ0
global IRQ1
global IRQ2
global IRQ3
global IRQ4
global IRQ5
global IRQ6
global IRQ7
global IRQ8
global IRQ9
global IRQ10
global IRQ11
global IRQ12
global IRQ13
global IRQ14
global IRQ15

irq_common_stub:
	; Save CPU state
	pushad

	; Save the data segment descriptor
	mov ax, ds
	push eax

	; Load the kernel data segment descriptor
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push esp
	cld
	call IRQHandler
	pop eax

	; Restore Data segment descriptor
	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popad
	add esp, 8 ; Cleanup pushed error code and pushed IRQ No
	iret

; IRQ HANDLERS
IRQ0:
	push byte 0
	push byte 32
	jmp irq_common_stub

IRQ1:
	push byte 0
	push byte 33
	jmp irq_common_stub

IRQ2:
	push byte 0
	push byte 34
	jmp irq_common_stub

IRQ3:
	push byte 0
	push byte 35
	jmp irq_common_stub

IRQ4:
	push byte 0
	push byte 36
	jmp irq_common_stub

IRQ5:
	push byte 0
	push byte 37
	jmp irq_common_stub

IRQ6:
	push byte 0
	push byte 38
	jmp irq_common_stub
	
IRQ7:
	push byte 0
	push byte 39
	jmp irq_common_stub

IRQ8:
	push byte 0
	push byte 40
	jmp irq_common_stub

IRQ9:
	push byte 0
	push byte 41
	jmp irq_common_stub

IRQ10:
	push byte 0
	push byte 42
	jmp irq_common_stub

IRQ11:
	push byte 0
	push byte 43
	jmp irq_common_stub

IRQ12:
	push byte 0
	push byte 44
	jmp irq_common_stub

IRQ13:
	push byte 0
	push byte 45
	jmp irq_common_stub

IRQ14:
	push byte 0
	push byte 46
	jmp irq_common_stub

IRQ15:
	push byte 0
	push byte 47
	jmp irq_common_stub
