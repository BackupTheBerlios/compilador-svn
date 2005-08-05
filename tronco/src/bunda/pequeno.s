	.file	"pequeno.c"
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
	.string	"%d\n"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	andl	$-16, %esp
	movl	$0, %eax
	subl	%eax, %esp
	movl	$2, a	#  a
	movl	a, %eax	#  a
	sall	$2, %eax
	movl	%eax, b	#  b
	subl	$8, %esp
	movl	b, %eax	#  b
	addl	a, %eax	#  a
	pushl	%eax
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	leave
	ret
	.size	main, .-main
	.comm	a,4,4
	.comm	b,4,4
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.3.5 (Debian 1:3.3.5-8ubuntu2)"
