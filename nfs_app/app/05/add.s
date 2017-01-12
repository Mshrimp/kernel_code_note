
	.section	.rodata
	.align	2
.LC0:
	.string	"val = %d\n"

	.section	.text
	.align	2
	.global	main
main:
	mov r10, lr	

	mov r3, #100
	mov r2, #1
	add r1, r2, r3, lsl #1
	ldr r0, =.LC0
	bl	printf

	mov r0, #0
	mov lr, r10
	mov pc, lr
