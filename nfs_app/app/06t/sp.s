

	.section	.rodata
	.align	2

.PRINT:
	.string	"val = %d\n"
	.align	2

.SCANF:
	.string	"%d"
	.align	2


	.section	.text
	.align	2
	.global	main

main:
	mov r10, lr

	@int i
	sub sp, sp, #4
	@mov r2, #1
	@str r2, [sp]

	ldr r0, =.SCANF
	mov r1, sp
	bl scanf

	ldr r1, [sp]
	ldr r0, =.PRINT
	bl printf

	add sp, sp, #4


	mov r0, #0
	mov lr, r10
	mov pc, lr


