@输出1-100之间的奇数

	.section .rodata
	.align 2

.PRINT:
	.string "%d\t"

.ENTER:
	.string "\n"

	.section .text
	.align 2
	.global main


main:
	mov r10, lr

	@for (i = 1; i <= 100; i++)
	@{
	@	if (i % 2 != 0)
	@	{
	@		count++;
	@		printf("%d\t");
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
	bgt		forout
	
	mov r2, r4
	ands r2, r2, #1
	cmp r2, #0
	bne		num
	b		numover
num:
	mov r1, r4
	ldr r0, =.PRINT
	bl		printf

	add r5, #1

	cmp r5, #5
	bleq	if
	b		ifover
if:
	mov r5, #0
	ldr r0, =.ENTER
	bl		printf
ifover:
numover:
	add r4, #1
	b		for
forout:

	mov r0, #0
	mov lr, r10
	mov pc, lr



