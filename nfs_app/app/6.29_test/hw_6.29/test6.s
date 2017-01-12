
@把r1 = 0xff37 第3~12位设置成0x327

	.section	.rodata
	.align	2

.PRINT:
	.string	"%p %p %p\n"

	.section	.text
	.align	2
	.global	main

main:
	mov r10, lr

	mov r1, #0xff00
	add r1, r1, #0x37

	mov r2, #0x300
	add r2, r2, #0x27

	lsl r2, #3

	mov r3, #0x300
	add r3, r3, #0xff
	lsl r3, #3
	
	bic r1, r1, r3
	@add r1, r1, r2
	orr r1, r1, r2

	ldr r0, =.PRINT
	bl		printf



	mov r0, #0
	mov lr, r10
	mov pc, lr

