	.file	"1.c"
	.comm	e1,4,4
	.comm	e2,4,4
	.comm	x,4,4
	.comm	y,4,4
	.comm	z,4,4
	.globl	overflow3
	.bss
	.align 4
	.type	overflow3, @object
	.size	overflow3, 4
overflow3:
	.zero	4
	.globl	overflow2
	.align 4
	.type	overflow2, @object
	.size	overflow2, 4
overflow2:
	.zero	4
	.globl	overflow1
	.align 4
	.type	overflow1, @object
	.size	overflow1, 4
overflow1:
	.zero	4
	.globl	overflow
	.align 4
	.type	overflow, @object
	.size	overflow, 4
overflow:
	.zero	4
	.comm	g_string,5,1
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$1424, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -1416(%rbp)
	movl	$1, -1412(%rbp)
	movl	$2, y(%rip)
	movl	$3, z(%rip)
	movl	$10, -1416(%rbp)
	movl	-1416(%rbp), %eax
	movl	%eax, %edi
	call	func1
	movl	%eax, -1416(%rbp)
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L3
	call	__stack_chk_fail
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.globl	recursion
	.type	recursion, @function
recursion:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	cmpl	$0, -4(%rbp)
	je	.L7
	movl	-4(%rbp), %eax
	movl	%eax, a.2847(%rip)
	movl	-4(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %edi
	call	recursion
	jmp	.L4
.L7:
	nop
.L4:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	recursion, .-recursion
	.globl	func1
	.type	func1, @function
func1:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movl	$100, -4(%rbp)
	movl	x(%rip), %edx
	movl	-20(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, x(%rip)
	movl	-20(%rbp), %eax
	addl	%eax, -4(%rbp)
	movl	z(%rip), %edx
	movl	-20(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, z(%rip)
	leaq	-20(%rbp), %rax
	movq	%rax, %rdi
	call	func2
	movl	-20(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	func1, .-func1
	.globl	func2
	.type	func2, @function
func2:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	$1000, -4(%rbp)
	movq	-24(%rbp), %rax
	movl	(%rax), %edx
	movl	x(%rip), %eax
	addl	%edx, %eax
	movl	%eax, x(%rip)
	movq	-24(%rbp), %rax
	movl	(%rax), %edx
	movl	y(%rip), %eax
	addl	%edx, %eax
	movl	%eax, y(%rip)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	addl	%eax, -4(%rbp)
	movq	-24(%rbp), %rax
	movl	$0, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	func2, .-func2
	.local	a.2847
	.comm	a.2847,4,4
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
