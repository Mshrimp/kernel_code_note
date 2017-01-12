
	.section	.rodata
	.align	2
.LC0:
	.string	"val = %d\n"

	.section	.text
	.align	2
	.global	main
main:
	mov r10, lr	
	
	@mov r1, #(1023 * 23 - (34 + 12) >> 3 | (1<<14) + 3487 & (0x37<<12) ^ 0xff)

	mov r1, #1024
	sub r1, #1
	mov r2, #23
	mul r1, r1, r2

	mov r2, #34
	add r2, #12

	sub r1, r2
	mov r1, r1, lsr #3

	mov r2, #1
	@mov r2, r2, lsl #14
	lsl r2, r2, #14
	
	mov r3, #0xd90
	add r3, #0xf
	add r2, r3
	
	mov r3, #0x37
	lsl r3, #12

	and r2, r2, r3
	eor r2, #0xff

	orr r1, r1, r2
	ldr r0, =.LC0
	bl	printf

	mov r0, #0
	mov lr, r10
	mov pc, lr
