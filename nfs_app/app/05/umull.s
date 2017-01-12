
	.section	.rodata
	.align	2
.LC0:
	.string	"val = %p %p\n"

	.section	.text
	.align	2
	.global	main
main:
	mov r10, lr	
	
	mov r4, #0xffffffff
	mov r3, #2
	umull r2, r1, r3, r4
	ldr r0, =.LC0
	bl	printf

	mov r0, #0
	mov lr, r10
	mov pc, lr
