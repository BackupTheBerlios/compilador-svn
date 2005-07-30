	.file	"teste.c"
 # GNU C version 3.4.2 (mingw-special) (mingw32)
 #	compiled by GNU C version 3.4.2 (mingw-special).
 # GGC heuristics: --param ggc-min-expand=47 --param ggc-min-heapsize=32702
 # options passed:  -iprefix -march=i386 -auxbase -O0 -fverbose-asm
 # options enabled:  -feliminate-unused-debug-types -fpeephole
 # -ffunction-cse -fkeep-static-consts -freg-struct-return -fgcse-lm
 # -fgcse-sm -fgcse-las -fsched-interblock -fsched-spec
 # -fsched-stalled-insns -fsched-stalled-insns-dep -fbranch-count-reg
 # -fcommon -fverbose-asm -fargument-alias -fzero-initialized-in-bss
 # -fident -fmath-errno -ftrapping-math -m80387 -mhard-float
 # -mno-soft-float -malign-double -mieee-fp -mfp-ret-in-387
 # -mstack-arg-probe -mno-red-zone -mtune=i386 -march=i386

	.section .rdata,"dr"
LC0:
	.ascii "James Brown\0"
LC1:
	.ascii "[%d  %s  %d %d]\0"
	.text
.globl _display
	.def	_display;	.scl	2;	.type	32;	.endef
_display:
	pushl	%ebp	 #
	movl	%esp, %ebp	 #,
	subl	$8, %esp	 #,
	subl	$12, %esp	 #,
	pushl	$8	 #
	call	_fatorial2	 #
	addl	$4, %esp	 #,
	pushl	%eax	 # tmp58
	pushl	_currentYear	 # currentYear
	pushl	$LC0	 #
	pushl	$8	 #
	pushl	$LC1	 #
	call	_printf	 #
	addl	$32, %esp	 #,
	leave
	ret
.globl _fatorial
	.def	_fatorial;	.scl	2;	.type	32;	.endef
_fatorial:
	pushl	%ebp	 #
	movl	%esp, %ebp	 #,
	subl	$4, %esp	 #,
	cmpl	$0, 8(%ebp)	 #, param
	jne	L3	 #,
	movl	$1, -4(%ebp)	 #, <result>
	jmp	L2	 #
L3:
	subl	$12, %esp	 #,
	movl	8(%ebp), %eax	 # param, tmp61
	decl	%eax	 # tmp61
	pushl	%eax	 # tmp61
	call	_fatorial	 #
	addl	$16, %esp	 #,
	imull	8(%ebp), %eax	 # param, tmp60
	movl	%eax, -4(%ebp)	 # tmp60, <result>
L2:
	movl	-4(%ebp), %eax	 # <result>, <result>
	leave
	ret
.globl _fatorial2
	.def	_fatorial2;	.scl	2;	.type	32;	.endef
_fatorial2:
	pushl	%ebp	 #
	movl	%esp, %ebp	 #,
	subl	$4, %esp	 #,
	movl	$1, -4(%ebp)	 #, result
L6:
	cmpl	$1, 8(%ebp)	 #, param1
	jle	L7	 #,
	movl	-4(%ebp), %eax	 # result, result
	imull	8(%ebp), %eax	 # param1, tmp60
	movl	%eax, -4(%ebp)	 # tmp60, result
	decl	8(%ebp)	 # param1
	jmp	L6	 #
L7:
	movl	-4(%ebp), %eax	 # result, result
	leave
	ret
	.def	___main;	.scl	2;	.type	32;	.endef
.globl _main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	pushl	%ebp	 #
	movl	%esp, %ebp	 #,
	subl	$8, %esp	 #,
	andl	$-16, %esp	 #,
	movl	$0, %eax	 #, tmp59
	addl	$15, %eax	 #, tmp60
	addl	$15, %eax	 #, tmp61
	shrl	$4, %eax	 #, tmp62
	sall	$4, %eax	 #, tmp64
	movl	%eax, -4(%ebp)	 # tmp64,
	movl	-4(%ebp), %eax	 #,
	call	__alloca
	call	___main	 #
	movl	$2005, _currentYear	 #, currentYear
	subl	$12, %esp	 #,
	pushl	$555	 #
	call	_display	 #
	addl	$16, %esp	 #,
	leave
	ret
	.comm	_currentYear, 16	 # 4
	.def	_fatorial2;	.scl	3;	.type	32;	.endef
	.def	_printf;	.scl	3;	.type	32;	.endef
