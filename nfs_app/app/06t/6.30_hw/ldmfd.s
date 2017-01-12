
	.section	.rodata
	.align	2
.LC0:
	.string	"value = %d %d %d\n"
.LC1:
	.string	"addr = %p\n"
.L1:
	.word	.LC0

	.section	.text
	.align	2
	.global	main
main:
	mov r10, lr		@保存_start返回地址到r10 


	adr r4, .L0

	mov r1, r4
	ldr r0, =.LC1
	bl	printf

	ldmfd r4!, {r1,r2,r3}
	ldr r0, =.LC0
	bl	printf

	mov r1, r4
	ldr r0, =.LC1
	bl	printf

	add sp, sp, #11		@释放空间;
	mov r0, #0
	mov lr, r10
	mov pc, lr

.L0:					@低地址
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	6			@高地址


