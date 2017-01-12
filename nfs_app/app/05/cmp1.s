
	.section	.rodata
	.align	2
.LC0:
	.string	"hello world\n"
	.align	2
.LC1:
	.string	"hello arm\n"

	.section	.text
	.align	2
	.global	main
main:
	moval r10, lr	

	@if(r1 == 1)
	@	printf("hello arm\n");
	@else 
	@	printf("hello world\n");

	moval r1, #1
	cmp r1, #1		@r1 - 1 s-> cpsr
					@		 Z = 1
					@		 Z = 0
	ldreq r0, =.LC1
	ldrne r0, =.LC0

	bl	printf

	mov r0, #0
	mov lr, r10
	mov pc, lr
