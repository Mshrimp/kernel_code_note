

	.section	.rodata
	.align 2

.PRINT:
	.string "val = %d\n"


	.section	.text
	.align 2

	.global	main

main:
	mov r10, lr

	@1023 * 23
	mov r2, #1000
	mov r3, #23
	add r1, r2, r3
	mul r1, r1, r3

	@34 + 12
	mov r3, #34
	mov r4, #12
	add r2, r3, r4

	@1023 * 23 - (34 + 12)
	sub r1, r1, r2

	@r1 = (1023 * 23 - (34 + 12)) >> 3
	mov r1, r1, lsr #3

	@(1 << 14)
	mov r2, #1
	mov r2, r2, lsl #14
	
	@3487
	mov r3, #2000
	mov r4, #1000
	add r3, r3, r4
	mov r4, #400
	add r3, r3, r4
	mov r4, #87
	add r3, r3, r4

	@r2 = (1 << 14) + 3487
	add r2, r2, r3

	@r3 = (0x37 << 12)
	mov r3, #0x37
	mov r3, r3, lsl #12
	
	@r4 = 0xff
	mov r4, #0xff

	and r2, r2, r3
	eor r2, r2, r4 
	orr r1, r1, r2

	ldr r0, =.PRINT
	bl	printf

	mov r0, #0
	mov lr, r10
	mov pc, lr


