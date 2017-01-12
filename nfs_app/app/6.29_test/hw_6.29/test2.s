
@r1 = oxff
@把r1第3位清零

	.section .rodata
	.align 2

.PRINT:
	.string "val = %p %p\n"

	.section .text
	.align 2
	.global main



main:
	mov r10, lr

	mov r1, #0xff
	mov r2, #1
	lsl r2, #3

	bic r1, r1, r2
	
	ldr r0, =.PRINT
	bl printf

	mov r0, #0
	mov lr, r10
	mov pc, lr



