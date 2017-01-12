
@把r1 = 0xff37 第3~12位设置成取反

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
	add r2, r2, #0xff
	lsl r2, #3
	
	eor r1, r1, r2

	ldr r0, =.PRINT
	bl		printf

	mov r0, #0
	mov lr, r10
	mov pc, lr

