section .text

extern FaultHandler

global ISR0
global ISR1
global ISR2
global ISR3
global ISR4
global ISR5
global ISR6
global ISR7
global ISR8
global ISR9
global ISR10
global ISR11
global ISR12
global ISR13
global ISR14
global ISR15
global ISR16
global ISR17
global ISR18
global ISR19
global ISR20
global ISR21
global ISR22
global ISR23
global ISR24
global ISR25
global ISR26
global ISR27
global ISR28
global ISR29
global ISR30
global ISR31

isr_common_stub:
	; Save CPU State (Push EAX, ECX, EDX, EBX, original ESP, EBP, ESI, and EDI.)
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
	cld ; C code following the sysV ABI requires DF to be clear on function entry
	call FaultHandler
	pop eax

	; Restore Data segment descriptor
	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popad
	add esp, 8 ; Cleanup pushed error code and pushed ISR No
	iret
	
; ISR HANDLERS
; 0: Divide By Zero Exception
ISR0:
    push byte 0
    push byte 0
    jmp isr_common_stub

; 1: Debug Exception
ISR1:
    push byte 0
    push byte 1
    jmp isr_common_stub

; 2: Non Maskable Interrupt Exception
ISR2:
    push byte 0
    push byte 2
    jmp isr_common_stub

; 3: Int 3 Exception
ISR3:
    push byte 0
    push byte 3
    jmp isr_common_stub

; 4: INTO Exception
ISR4:
    push byte 0
    push byte 4
    jmp isr_common_stub

; 5: Out of Bounds Exception
ISR5:
    push byte 0
    push byte 5
    jmp isr_common_stub

; 6: Invalid Opcode Exception
ISR6:
    push byte 0
    push byte 6
    jmp isr_common_stub

; 7: Coprocessor Not Available Exception
ISR7:
    push byte 0
    push byte 7
    jmp isr_common_stub

; 8: Double Fault Exception (With Error Code!)
ISR8:
	push byte 0
    push byte 8
    jmp isr_common_stub

; 9: Coprocessor Segment Overrun Exception
ISR9:
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Bad TSS Exception (With Error Code!)
ISR10:
	push byte 0
    push byte 10
    jmp isr_common_stub

; 11: Segment Not Present Exception (With Error Code!)
ISR11:
	push byte 0
    push byte 11
    jmp isr_common_stub

; 12: Stack Fault Exception (With Error Code!)
ISR12:
	push byte 0
    push byte 12
    jmp isr_common_stub

; 13: General Protection Fault Exception (With Error Code!)
ISR13:
	push byte 0
    push byte 13
    jmp isr_common_stub

; 14: Page Fault Exception (With Error Code!)
ISR14:
	push byte 0
    push byte 14
    jmp isr_common_stub

; 15: Reserved Exception
ISR15:
    push byte 0
    push byte 15
    jmp isr_common_stub

; 16: Floating Point Exception
ISR16:
    push byte 0
    push byte 16
    jmp isr_common_stub

; 17: Alignment Check Exception
ISR17:
    push byte 0
    push byte 17
    jmp isr_common_stub

; 18: Machine Check Exception
ISR18:
    push byte 0
    push byte 18
    jmp isr_common_stub

; 19: Reserved
ISR19:
    push byte 0
    push byte 19
    jmp isr_common_stub

; 20: Reserved
ISR20:
    push byte 0
    push byte 20
    jmp isr_common_stub

; 21: Reserved
ISR21:
    push byte 0
    push byte 21
    jmp isr_common_stub

; 22: Reserved
ISR22:
    push byte 0
    push byte 22
    jmp isr_common_stub

; 23: Reserved
ISR23:
    push byte 0
    push byte 23
    jmp isr_common_stub

; 24: Reserved
ISR24:
    push byte 0
    push byte 24
    jmp isr_common_stub

; 25: Reserved
ISR25:
    push byte 0
    push byte 25
    jmp isr_common_stub

; 26: Reserved
ISR26:
    push byte 0
    push byte 26
    jmp isr_common_stub

; 27: Reserved
ISR27:
    push byte 0
    push byte 27
    jmp isr_common_stub

; 28: Reserved
ISR28:
    push byte 0
    push byte 28
    jmp isr_common_stub

; 29: Reserved
ISR29:
    push byte 0
    push byte 29
    jmp isr_common_stub

; 30: Reserved
ISR30:
    push byte 0
    push byte 30
    jmp isr_common_stub

; 31: Reserved
ISR31:
    push byte 0
    push byte 31
    jmp isr_common_stub
