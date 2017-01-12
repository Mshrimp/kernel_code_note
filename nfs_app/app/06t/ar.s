
@使用栈实现简单数组


@    int cmp_int(const void *d1, const void *d2)
@    {
@        if(*(int*)d1 > *(int*)d2)
@            return 1;
@        else
@            return 0;
@    }
@
@    int ar[100];
@    int br[100];
@    srandom(getpid());
@    for(i = 0; i < 100; i++)
@        ar[i] = random() % 500;
@
@    for(i = 0; i < 100; i++)
@        br[i] = ar[i];
@
@    qsort(br, 100, sizeof(int), cmp_int);
@    for(i = 0; i < 100; i++)
@        printf("ar[%d] = %d br[%d] = %d\n", i, ar[i], i, br[i]);
	
	.section	.rodata
	.align	2

.PRINT:
	.string	"ar[%d] = %d\tbr[%d] = %d\n"
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
	sub sp, sp, #(4*200)	@开辟空间，定义数组
	mov r5, sp				@r5指向数组ar首地址
	add r7, sp, #(4*100)	@r7指向数组br首地址

	mov r4, #0				@定义数组下标计数器
1:
	cmp r4, #100			@数组越界则跳出
	bge		1f		

	@生成小于500的随机数
	bl	random
rand:	
	cmp r0, #500
	blt	randout
	b	randsub
randsub:
	sub r0, #500
	b	rand

randout:

	str r0, [r5], #4	@将计数器的值作为数组元素存入数组中
						@[r5] = r4, r5 += 4
	add r4, #1			@数组下标递增

	b	1b
1:

	@将数组ar中的数据复制到数组br中
	mov r6, #0				@定义数组下标计数器
	mov r5, sp
1:
	cmp r6, #100			@数组越界则跳出
	bge		1f		

	ldr r1, [r5], #4		@将数组ar中的数据取出

	str r1, [r7], #4		@将取出的值作为数组元素存入数组br中
	add r6, #1				@数组下标递增

	b	1b
1:

	@打印数组ar和br中的数据
	mov r5, sp				@r5指向数组首地址
	add r7, sp, #(4*100)
	mov r4, #0
1:
	cmp r4, #100			
	bge		1f

	mov r1, r4
	ldr r2, [r5], #4	@打印数组元素，并将地址递增
	mov r3, r4
	ldr r8, [r7], #4
	push	{r8}
	ldr r0, =.PRINT
	bl printf

	pop		{r8}

	add r4, #1
	b		1b
1:

	add sp, sp, #(4*200)		@释放数组空间
	mov r0, #0
	mov lr, r10
	mov pc, lr


