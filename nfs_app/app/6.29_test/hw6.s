
@输出九九乘法表

	.section	.rodata
	.align	2

.PRINT:
	.string	"%d * %d = %2d  "
	.align	2
.ENTER:
	.string "\n"

	.section	.text
	.align	2
	.global	main

@#include <stdio.h>

@int main(void)
@{
@	int i, j;
@
@	for (i = 1; i < 10; i++)
@	{
@		for (j = 1; j <= i; j++)
@		{
@			printf("%d * %d = %2d  ", j, i, i * j);
@		}
@		printf("\n");
@	}
@
@	return 0;
@}

main:
	mov r10, lr

@int main(void)
@{
@	int i, j;
@		r4,r5


@	for (i = 1; i < 10; i++)
	mov r4, #1
for1:
	cmp r4, #10
	bge		out

@	{
@		for (j = 1; j <= i; j++)
		mov r5, #1
	for2:
		cmp r5, r4
		bgt		out1

@		{
@			printf("%d * %d = %2d  ", j, i, i * j);
@		}
		mov r1, r5
		mov r2, r4
		mul r3, r4, r5
		ldr r0, =.PRINT
		bl printf

		add r5, #1
		b		for2

@		printf("\n");
@	}
	
out1:
	ldr r0, =.ENTER
	bl		printf
	add r4, #1
	b		for1
@
@	return 0;
@}
out:


	mov r0, #0
	mov lr, r10
	mov pc, lr


