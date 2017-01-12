
	.section	.rodata
	.align	2
.LC0:
	.string	"val = %p\n"

	.section	.text
	.align	2
	.global	main
main:
	mov r10, lr	

	@mov r1, #(10-2*3)
	@mov r2, #2
	@mov r1, r2, lsl #2
	@mov r1, r2, lsr #1
	@mov r2, #0xff000000
	@mov r2, #0xffffffff
	@mov r1, r2, lsr #1
	@mov r1, r2, asr #1
	mov r2, #1
	mov r1, r2, ror #31
	ldr r0, =.LC0
	bl	printf

	mov r0, #0
	mov lr, r10
	mov pc, lr
