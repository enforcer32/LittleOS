section .text

global EnablePaging
global DisablePaging
global PagingLoadDirectory

EnablePaging:
	push ebp
	mov ebp, esp
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	pop ebp
	ret

DisablePaging:
	push ebp
	mov ebp, esp
	mov eax, cr0
	and eax, 0x7FFFFFFF
	mov cr0, eax
	pop ebp
	ret

PagingLoadDirectory:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr3, eax
	pop ebp
	ret
