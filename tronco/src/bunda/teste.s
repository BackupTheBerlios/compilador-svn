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
	jne	.L3
	movl	$1, -4(%ebp)
	jmp	.L2
.L3:
	subl	$12, %esp
	movl	8(%ebp), %eax	#  param
	decl	%eax
	pushl	%eax
	call	fatorial
	addl	$16, %esp
	imull	8(%ebp), %eax	#  param
	movl	%eax, -4(%ebp)
.L2:
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
.L6:
	cmpl	$1, 8(%ebp)	#  param1
	jg	.L8
	jmp	.L7
.L8:
	movl	-4(%ebp), %eax	#  result
	imull	8(%ebp), %eax	#  param1
	movl	%eax, -4(%ebp)	#  result
	decl	8(%ebp)	#  param1
	jmp	.L6
.L7:
	movl	-4(%ebp), %eax	#  result
	leave
	ret
	.size	fatorial2, .-fatorial2
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
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
	leave
	ret
	.size	main, .-main
	.comm	currentYear,4,4
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.3.5 (Debian 1:3.3.5-8ubuntu2)"
