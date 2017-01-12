
@使用栈实现简单数组

	.section	.rodata
	.align	2

.PRINT:
	.string	"val = %d\n"
	.align	2

.SCANF:
	.string	"%d"
	.align	2


	.section	.text
	.align	2
	.global	main

main:
	mov r10, lr		@保存_start返回地址到r10

	@int ar[5]		
	sub sp, sp, #20		@开辟空间，定义数组
	mov r5, sp			@r5指向数组首地址

	mov r4, #0			@定义数组下标计数器
1:
	cmp r4, #5			@数组越界则跳出
	bge		1f		

	str r4, [r5], #4	@将计数器的值作为数组元素存入数组中
						@[r5] = r4, r5 += 4
	add r4, #1			@数组下标递增

	b	1b
1:

	mov r5, sp			@r5指向数组首地址
	mov r4, #0
1:
	cmp r4, #5			
	bge		1f

	ldr r1, [r5], #4	@打印数组元素，并将地址递增
	ldr r0, =.PRINT
	bl printf

	add r4, #1
	b		1b
1:

	mov r5, sp			@r5指向数组首地址
	mov r4, #0
1:
	cmp r4, #5			
	bge		1f

	@ldr r1, [r5], #4	@打印数组元素，并将地址递增
	@ldr r1, [r5]
	ldr r1, [r5, r4, lsl #2] 	@打印数组元素，并将地址递增
								@r1 = [r5 + r4 * 4],按索引号乘以4字节打印
	ldr r0, =.PRINT
	bl printf

	add r4, #1
	@add r5, #4
	b		1b
1:

	mov r5, sp
	ldr r1, [r5, #8]!	@将数组首地址元素偏移8字节，指针位置加8
	ldr r0, =.PRINT
	bl printf

	ldr r1, [r5, #8]	@打印数组指针后8字节处的数据，指针位置不变
						@寻址偏移不能超过4096(4K)
	ldr r0, =.PRINT
	bl printf

	add sp, sp, #20		@释放数组空间
	mov r0, #0
	mov lr, r10
	mov pc, lr


