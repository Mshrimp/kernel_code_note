
@使用栈实现简单数组

@	int ar[100];
@	for(i = 0; i < 100; i++)
@		ar[i] = i;
@	for(i = 0; i < 100; i++)
@		printf("ar[%d] = %d\n", i, ar[i]);

	
	.section	.rodata
	.align	2

.PRINT:
	.string	"ar[%d] = %d\n"
	.align	2

.SCANF:
	.string	"%d"
	.align	2


	.section	.text
	.align	2
	.global	main

main:
	mov r10, lr		@保存_start返回地址到r10

	@int ar[100]		
	sub sp, sp, #(4*100)		@开辟空间，定义数组
	mov r5, sp			@r5指向数组首地址

	mov r4, #0			@定义数组下标计数器
1:
	cmp r4, #100			@数组越界则跳出
	bge		1f		

	str r4, [r5], #4	@将计数器的值作为数组元素存入数组中
						@[r5] = r4, r5 += 4
	add r4, #1			@数组下标递增

	b	1b
1:

	mov r5, sp			@r5指向数组首地址
	mov r4, #0
1:
	cmp r4, #100			
	bge		1f

	mov r1, r4
	ldr r2, [r5], #4	@打印数组元素，并将地址递增
	ldr r0, =.PRINT
	bl printf

	add r4, #1
	b		1b
1:

	add sp, sp, #(4*100)		@释放数组空间
	mov r0, #0
	mov lr, r10
	mov pc, lr


