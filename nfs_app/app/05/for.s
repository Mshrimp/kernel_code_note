
	.section	.rodata
	.align	2
.LC0:
	.string	"hello\n"

	.section	.text
	.align	2
	.global	main
main:
	moval r10, lr	

	@for(i = 0; i < 10; i++)
	@{
	@	printf("hello\n");
	@}
	
	mov r4, #0
for:
	cmp r4, #10
	bge	out

	ldr r0, =.LC0
	bl	printf

	add r4, #1
	b	for
out:

	mov r0, #0
	mov lr, r10
	mov pc, lr
