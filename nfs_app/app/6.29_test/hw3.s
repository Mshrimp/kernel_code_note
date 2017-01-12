@输出100个随机数

	.section .rodata
	.align 2

.PRINT:
	.string "%d\t"
	.align 2

.PRINT2:
	.string "val[%d] = %d\n"
	.align 2

.ENTER:
	.string "\n"

	.section .text
	.align 2
	.global main


main:
	mov r10, lr

	@for (i = 1; i <= 100; i++)
	@{
	@	num = rand() % 100
	@	if (i % 2 != 0)
	@	{
	@		count++;
	@		printf("%d\t", num);
	@		if (count % 5 == 0)
	@		{
	@			printf("\n");
	@		}
	@	}
	@}

	mov r4, #1
	mov r5, #0
for:
	cmp r4, #100
	bgt	out
	
	

	bl	 random
	mov r2, r0
	mov r1, r4
	ldr r0, =.PRINT2
	bl	printf

	add r4, #1

	add r5, #1

	cmp r5, #5
	bleq ifout
	b	ifover
ifout:
	mov r5, #0
	ldr r0, =.ENTER
	bl printf
ifover:
	b	for
out:


	mov r0, #0
	mov lr, r10
	mov pc, lr



