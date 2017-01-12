
@把r1 = 0xff00 第3, 5, 7位置1


	.section	.rodata
	.align	2

.PRINT:
	.string	"%p %p\n"


	.section	.text
	.align	2
	.global	main

main:
	mov r10, lr

	mov r1, #0xff00
	mov r2, #1

	lsl r2, #3
	orr r1, r1, r2

	lsl r2, #2
	orr r1, r1, r2

	lsl r2, #2
	orr r1, r1, r2

	ldr r0, =.PRINT
	bl		printf


	mov r0, #0
	mov lr, r10
	mov pc, lr

