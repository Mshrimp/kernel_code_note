
	.section .rodata
.STR:
	.string "Hello world!\n"

	.section .text
	.global main

main:
	mov r4, lr

	ldr r0, =.STR
	bl printf

	mov r0, #0
	mov lr, r4
	mov pc, lr


