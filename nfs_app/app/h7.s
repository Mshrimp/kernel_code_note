
	.section	.rodata
	.align	2
.LC0:
	.string	" "
	.align	2
.LC1:
	.string "*"
	.align	2
.LC2:
	.string	"\n"

	.section	.text
	.align	2
	.global	main
main:
	mov r10, lr

	@int i, j, k;
	@	r4 r5 r6

	@for(k = 1; k <= 10; k++)
	mov r6, #1
1:
	cmp	r6, #10
	bgt	1f
		
		@for(i = 0; i < k; i++)
		mov r4, #0
	2:
		cmp r4, r6
		bge	2f
			@for(j = 0; j < k-1-i; j++)
			mov r5, #0
		3:
			sub r1, r6, #1
			sub r1, r1, r4
			cmp	r5, r1
			bge	3f	
				@printf(" ");
				ldr r0, =.LC0
				bl	printf
			add r5, #1
			b	3b
		3:

			@for(j = 0; j < i*2+1; j++)
			mov r5, #0
		3:
			lsl r1, r4, #1
			add r1, #1
			cmp	r5, r1
			bge	3f	
				@printf("*");
				ldr r0, =.LC1
				bl	printf
			add r5, #1
			b	3b
		3:
			@printf("\n");
			ldr r0, =.LC2
			bl	printf

		add r4, #1
		b	2b
	2:

		@for(i = 0; i < k-1; i++)
		mov r4, #0
	2:
		sub r1, r6, #1
		cmp r4, r1
		bge 2f
	
		@	for(j = 0; j <= i; j++)
			mov r5, #0
		3:
			cmp r5, r4
			bgt 3f
		
		@		printf(" ");
			ldr r0, =.LC0
			bl printf

			add r5, #1
			b	3b
		3:
		@	for(j = 0; j < 2*(k-i-1)-1; j++)
			mov r5, #0
		3:
			sub r1, r6, r4
			sub r1, r1, #1
			lsl	r1, #1
			sub r1, r1, #1
			cmp r5, r1
			bge	3f
		@		printf("*");
			ldr r0, =.LC1
			bl printf

			add r5, #1
			b	3b
		3:
		@	printf("\n");
			ldr r0, =.LC2
			bl printf

		add r4, #1
		b	2b
	2:

	add r6, #1
	b	1b
1:

	mov r0, #0
	mov lr, r10
	mov pc, lr
