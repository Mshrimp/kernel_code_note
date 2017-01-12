
	.section	.rodata
	.align	2
.LC0:
	.string	"val = %d\n"

	.section	.text
	.align	2
	.global	main
main:
	mov r10, lr	

	mrs r1, cpsr
	bic r1, r1, #(1<<29)
	msr cpsr, r1

	mov r2, #0xffffffff
	adds r2, #1

	mov r3, #20
	mov r2, #10
	rsc r1, r2, r3

	ldr r0, =.LC0
	bl	printf

	mov r0, #0
	mov lr, r10
	mov pc, lr
