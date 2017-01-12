
	.section .rodata
.LC0:
	.string	"hello world\n"

	.section .text
	.global	main	@导出符号main
					@伪指令;
main:				@符号/标签, 地址;
	mov r10, lr		@保存调用者_start的返回地址;

	@printf("hello world");
	ldr r0, =.LC0	@r0=.LC0
	bl	printf		@lr =当前指令下一条地址;
					@跳转指令, 跳转到printf入口;

	@return 0
	mov r0, #0		@返回值;
	mov lr, r10
	mov pc, lr		@返回程序;



