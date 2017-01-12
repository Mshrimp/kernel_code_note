
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

	mov r1, #20

	@if(r1 > 10)
	@{
	@	printf("r1 > 10\n");
	@} 
	@else if(r1 < 10)
	@{
	@	printf("r1 < 10\n");
	@}
	@else
	@{
	@	printf("r1 == 10\n");
	@}

	cmp r1, #10
	bgt	do1
	blt	do2
	b	do3
do1:
	ldr	r0, =.LC0
	bl	printf
	b	out
do2:
	ldr	r0, =.LC1
	bl	printf
	b	out
do3:
	ldr	r0, =.LC2
	bl	printf
out:

	mov r0, #0
	mov lr, r10
	mov pc, lr
