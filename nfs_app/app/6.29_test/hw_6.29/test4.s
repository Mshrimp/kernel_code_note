
@把r1 = 0xf0的第3位置1

	.section .rodata
	.align 2

.PRINT:
	.string "val = %p %p\n"

.BINPRINT:
	.string "%d"

.SPACEPRINT:
	.string " "

	.section .text
	.align 2
	.global main

main:
	mov r10, lr

	mov r1, #0xf0
	mov r2, #1
	lsl r2, #3

	orr r1, r1, r2

	mov r4, r1

	ldr r0, =.PRINT
	bl		printf

	mov r6, #0x80000000
	mov r5, #0
	mov r7, #0
for:
	cmp r5, #31
	bgt		forout

	and r1, r4, r6

	lsr r1, #31

	ldr r0, =.BINPRINT
	bl		printf
	add r7, #1

	cmp r7, #4
	bleq	space
	b		spaceout
space:
	ldr r0, =.SPACEPRINT
	bl		printf
	mov r7, #0
spaceout:
	lsl r4, #1
	add r5, #1
	b		for

forout:


	mov r0, #0
	mov lr, r10
	mov pc, lr



