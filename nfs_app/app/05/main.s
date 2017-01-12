	.cpu arm1176jzf-s
	.eabi_attribute 27, 3
	.fpu vfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"main.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"a > 10\000"
	.align	2
.LC1:
	.ascii	"a < 10\000"
	.align	2
.LC2:
	.ascii	"a == 10\000"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r3, r4, fp, lr}
	add	fp, sp, #12
	mov	r4, #10
	cmp	r4, #10
	ble	.L2
	ldr	r0, .L5
	bl	puts
	b	.L3
.L2:
	cmp	r4, #9
	bgt	.L4
	ldr	r0, .L5+4
	bl	puts
	b	.L3
.L4:
	ldr	r0, .L5+8
	bl	puts
.L3:
	mov	r3, #0
	mov	r0, r3
	ldmfd	sp!, {r3, r4, fp, pc}
.L6:
	.align	2
.L5:
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.size	main, .-main
	.ident	"GCC: (ctng-1.8.1-FA) 4.5.1"
	.section	.note.GNU-stack,"",%progbits
