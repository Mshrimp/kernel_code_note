
	.section	.rodata
	.align	2
.LC0:
	.string	"hello\n"

	.section	.text
	.align	2
	.global	main
main:
	moval r10, lr	

	@while(1)
	@{
	@	printf("hello\n");
	@	sleep(1);
	@}
	
loop:
	ldr r0, =.LC0
	bl	printf
	mov r0, #1
	bl	sleep
	b	loop


	mov r0, #0
	mov lr, r10
	mov pc, lr
