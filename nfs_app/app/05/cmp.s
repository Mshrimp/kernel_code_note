
	.section	.rodata
	.align	2
.LC0:
	.string	"r > 10\n"
	.align	2
.LC1:
	.string	"r < 10\n"
	.align	2
.LC2:
	.string	"r == 10\n"

	.section	.text
	.align	2
	.global	main
main:
	moval r10, lr	

	@if(r1 > 10)
	@	printf("r1 > 10\n");
	@else if(r1 < 10)
	@	printf("r1 < 10\n");
	@else
	@	printf("r1 == 10\n");

	mov r1, #10
	@subs r1, r1, #10
	@N = 1 负 N = 0 正
	@  <		>
	@Z = 1 零 Z = 0 非零
	@  == 		!=

	cmp r1, #10
	ldrgt r0, =.LC0		
	ldrlt r0, =.LC1
	ldreq r0, =.LC2
	bl	printf

	mov r0, #0
	mov lr, r10
	mov pc, lr
