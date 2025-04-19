	.file	1 "s3.c"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module	fp=32
	.module	nooddspreg
	.abicalls
	.text
	.globl	a
	.section	.bss,"aw",@nobits
	.align	2
	.type	a, @object
	.size	a, 4
a:
	.space	4
	.globl	b
	.align	2
	.type	b, @object
	.size	b, 4
b:
	.space	4
	.globl	c
	.align	2
	.type	c, @object
	.size	c, 4
c:
	.space	4
	.globl	d
	.align	2
	.type	d, @object
	.size	d, 4
d:
	.space	4
	.rdata
	.align	2
$LC0:
	.ascii	"%d%d%d\000"
	.align	2
$LC1:
	.ascii	"%d\000"
	.text
	.align	2
	.globl	main
	.set	nomips16
	.set	nomicromips
	.ent	main
	.type	main, @function
main:
	.frame	$fp,32,$31		# vars= 0, regs= 2/0, args= 16, gp= 8
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	reorder
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$fp,24($sp)
	move	$fp,$sp
	.cprestore	16
	la	$7,c
	la	$6,b
	la	$5,a
	la	$4,$LC0
	jal	__isoc99_scanf
	la	$2,a
	lw	$3,0($2)
	la	$2,b
	lw	$2,0($2)
	#nop
	subu	$3,$3,$2
	la	$2,c
	lw	$2,0($2)
	#nop
	subu	$3,$3,$2
	la	$2,d
	sw	$3,0($2)
	la	$2,d
	lw	$2,0($2)
	#nop
	move	$5,$2
	la	$4,$LC1
	jal	printf
	move	$2,$0
	move	$sp,$fp
	lw	$31,28($sp)
	lw	$fp,24($sp)
	addiu	$sp,$sp,32
	j	$31
	.end	main
	.size	main, .-main
	.ident	"GCC: (Ubuntu 10.3.0-1ubuntu1) 10.3.0"
	.section	.note.GNU-stack,"",@progbits
