
	.section	.rodata
	.align	2
.LC0:
	.string	"val = %p\n"

	.section	.text
	.align	2
	.global	main
main:
	mov r10, lr	

	mrs r1, cpsr
	@bic r1, r1, #(1<<29)
	mov r1, #0xffffffff
	msr cpsr, r1

	mrs r1, cpsr
	ldr r0, =.LC0
	bl	printf

	mov r0, #0
	mov lr, r10
	mov pc, lr
