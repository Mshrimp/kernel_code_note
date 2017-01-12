
	.section	.rodata
	.align	2
.LC0:
	.string	"val = %p\n"

	.section	.text
	.align	2
	.global	main
main:
	mov r10, lr	

	mov r2, #0xf5
	eor r1, r2, #0x3c
	ldr r0, =.LC0
	bl	printf

	mov r0, #0
	mov lr, r10
	mov pc, lr
