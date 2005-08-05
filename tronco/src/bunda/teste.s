	.file	"teste.c"
# GNU C version 3.3.5 (Debian 1:3.3.5-8ubuntu2) (i486-linux)
#	compiled by GNU C version 3.3.5 (Debian 1:3.3.5-8ubuntu2).
# GGC heuristics: --param ggc-min-expand=47 --param ggc-min-heapsize=32101
# options passed:  -D__GNUC__=3 -D__GNUC_MINOR__=3 -D__GNUC_PATCHLEVEL__=5
# -march=i386 -auxbase -O0 -fverbose-asm
# options enabled:  -fpeephole -ffunction-cse -fkeep-static-consts
# -fpcc-struct-return -fgcse-lm -fgcse-sm -fsched-interblock -fsched-spec
# -fbranch-count-reg -fcommon -fverbose-asm -fgnu-linker -fargument-alias
# -fzero-initialized-in-bss -fident -fmath-errno -ftrapping-math -m80387
# -mhard-float -mno-soft-float -mieee-fp -mfp-ret-in-387 -mcpu=i386
# -march=i386

	.text
.globl ret_dois_local_dois_param
	.type	ret_dois_local_dois_param, @function
ret_dois_local_dois_param:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	8(%ebp), %eax	#  a
	negl	%eax
	movl	%eax, -4(%ebp)	#  c
	movl	-4(%ebp), %eax	#  c
	leal	(%eax,%eax), %ecx
	movl	-4(%ebp), %edx	#  c
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	subl	8(%ebp), %eax	#  a
	addl	$4, %eax
	imull	12(%ebp), %eax	#  b
	leal	(%eax,%ecx), %edx
	movl	-4(%ebp), %eax	#  c
	imull	%edx, %eax
	movl	%eax, -8(%ebp)	#  d
	movl	-8(%ebp), %eax	#  d
	leave
	ret
	.size	ret_dois_local_dois_param, .-ret_dois_local_dois_param
	.section	.rodata
.LC0:
	.string	"James Brown"
.LC1:
	.string	"[%d  %s  %d %d]"
	.text
.globl display
	.type	display, @function
display:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$12, %esp
	pushl	$8
	call	fatorial2
	addl	$4, %esp
	pushl	%eax
	pushl	currentYear	#  currentYear
	pushl	$.LC0
	pushl	$8
	pushl	$.LC1
	call	printf
	addl	$32, %esp
	leave
	ret
	.size	display, .-display
.globl fatorial
	.type	fatorial, @function
fatorial:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	cmpl	$0, 8(%ebp)	#  param
	jne	.L4
	movl	$1, -4(%ebp)
	jmp	.L3
.L4:
	subl	$12, %esp
	movl	8(%ebp), %eax	#  param
	decl	%eax
	pushl	%eax
	call	fatorial
	addl	$16, %esp
	imull	8(%ebp), %eax	#  param
	movl	%eax, -4(%ebp)
.L3:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	fatorial, .-fatorial
.globl fatorial2
	.type	fatorial2, @function
fatorial2:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	$1, -4(%ebp)	#  result
.L7:
	cmpl	$1, 8(%ebp)	#  param1
	jg	.L9
	jmp	.L8
.L9:
	movl	-4(%ebp), %eax	#  result
	imull	8(%ebp), %eax	#  param1
	movl	%eax, -4(%ebp)	#  result
	decl	8(%ebp)	#  param1
	jmp	.L7
.L8:
	movl	-4(%ebp), %eax	#  result
	leave
	ret
	.size	fatorial2, .-fatorial2
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	andl	$-16, %esp
	movl	$0, %eax
	subl	%eax, %esp
	movl	$2005, -4(%ebp)	#  ano
	movl	$555, -8(%ebp)	#  num
	movl	-4(%ebp), %eax	#  ano
	movl	%eax, currentYear	#  currentYear
	subl	$12, %esp
	pushl	-8(%ebp)	#  num
	call	display
	addl	$16, %esp
	movl	$50, -20(%ebp)	#  k
	movl	$50, -16(%ebp)	#  j
	movl	$50, -12(%ebp)	#  i
	leave
	ret
	.size	main, .-main
	.comm	currentYear,4,4
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.3.5 (Debian 1:3.3.5-8ubuntu2)"
