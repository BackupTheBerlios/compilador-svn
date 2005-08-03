

_$init:

	.type _main, function
_main:
	pushl	%ebp
	movl	%esp,%ebp
    
//	pushl	%edi

	movl	$_$str_0,_name
	movl	$2005,_currentYear
	pushl $ 555
	call _display
	addl $4,%esp
_$prepare_end_main:
	popl	%edi
	popl	%ebp
	ret
_$end_main:
	.globl	_main
	
	
	.type _display, function
_display:
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%edi

	pushl $ 8
	call _fatorial2
	addl $4,%esp
    
	pushl %eax
	pushl _currentYear
	pushl _name
	pushl 8(%ebp)
	pushl $_$str_1
	call _printf
	addl $20,%esp
    
_$prepare_end_display:
	popl	%edi
	popl	%ebp
	ret
_$end_display:
	.globl	_display
	
	
	.type _fatorial, function
_fatorial:
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%edi

	movl	8(%ebp),%eax
	movl	%eax,%esi
	movl	$0,%eax
	movl	%eax,%edi
	cmpl	%esi,%edi
	jne		_$else_1
	movl	$1,%eax
	jmp		_$prepare_end_fatorial
_$else_1:
	subl	$1,%esi
	pushl %esi
	call _fatorial
	addl $4,%esp
	imull	8(%ebp),%eax
_$prepare_end_fatorial:
	popl	%edi
	popl	%ebp
	ret
_$end_fatorial:
	.globl	_fatorial
	
	
	
	.type _fatorial2, function
_fatorial2:
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%edi

	movl	$1,%eax
	movl	%eax,-4(%ebp)
_$begin_while_1:
	movl	8(%ebp),%eax
	movl	%eax,%esi
	movl	$1,%eax
	movl	%eax,%edi
	cmpl	%esi,%edi
	jg	_$while_end_1
	movl	-4(%ebp),%eax
	imull	8(%ebp),%eax
	movl	%eax,-4(%ebp)
	movl	8(%ebp),%eax
	subl	$1,%eax
	movl	%eax,8(%ebp)
	jmp	_$begin_while_1
_$while_end_1:
	movl	-4(%ebp),%eax
	jmp		_$prepare_end_fatorial2
_$prepare_end_fatorial2:
	popl	%edi
	popl	%ebp
	ret
_$end_fatorial2:
	.globl	_fatorial2
	
	
	
	.globl	_name
	.type	_name,object
	.comm	_name,4
	.globl	_currentYear
	.type	_currentYear,object
	.comm	_currentYear,4
	.extern	_printf
	
	
	
	.data
_$str_0:
	.byte 74,97,109,101,115,32,66,114,111,119,110,0
_$str_1:
	.byte 91,37,100,32,32,37,115,32,32,37,100,32,37,100,93,0
