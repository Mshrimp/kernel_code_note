
	.section .rodata
	.align 2

.PRINT:
	.string "val = %d %d %p %d \n"
	@.string "val = %d %d %d %d \n"

	.section .text
	.align 2
	.global main

main:
	mov r10, lr

	mov r7, #700
	mov r6, #600
	mov r5, #500
	mov r4, #400
	mov r3, #0x3fc00000
	@mov r3, #0xff >> 10
	mov r2, #200
	mov r1, #100
	push {r4, r5, r6, r7}
	@push {r1}
	ldr r0, =.PRINT
	bl	printf

	pop	{r1, r2, r3}
	push {r4}
	ldr r0, =.PRINT
	bl	printf
	pop {r4}

	mov r0, #0
	mov lr, r10
	mov pc, lr

