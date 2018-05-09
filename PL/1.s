	.file	"1.c"
	.comm	e1,4,4
	.comm	e2,4,4
	.globl	x
	.data
	.align 4
	.type	x, @object
	.size	x, 4
x:
	.long	1
	.globl	y
	.align 4
	.type	y, @object
	.size	y, 4
y:
	.long	10
	.globl	z
	.align 4
	.type	z, @object
	.size	z, 4
z:
	.long	100
	.comm	ch,1530,32
	.globl	s
	.align 32
	.type	s, @object
	.size	s, 60
s:
	.string	"INA HOHO"
	.zero	11
	.string	"NANANA"
	.zero	13
	.string	"DIFFERNT_LENGTH"
	.zero	4
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
	subq	$48, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movss	.LC0(%rip), %xmm0
	movss	%xmm0, -24(%rbp)
	movl	$4, -32(%rbp)
	movl	-32(%rbp), %eax
	addl	$5, %eax
	movl	%eax, -20(%rbp)
	movl	-32(%rbp), %edx
	movl	-20(%rbp), %eax
	addl	%edx, %eax
	subl	$1, %eax
	movl	%eax, -16(%rbp)
	leaq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	func
	movl	%eax, %edx
	movl	-32(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, y(%rip)
	movl	-32(%rbp), %eax
	movl	%eax, z(%rip)
	movb	$10, ch+1371(%rip)
	movl	-20(%rbp), %eax
	movl	%eax, %edi
	call	recursion
	movl	$0, -28(%rbp)
	jmp	.L2
.L3:
	movl	-28(%rbp), %eax
	addl	%eax, %eax
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, %edi
	movl	$0, %eax
	call	func_static
	addl	$1, -28(%rbp)
.L2:
	cmpl	$4, -28(%rbp)
	jle	.L3
	movl	-32(%rbp), %eax
	movl	-20(%rbp), %edx
	movl	%edx, %esi
	movl	%eax, %edi
	call	func_ext
	movl	-32(%rbp), %eax
	movl	-24(%rbp), %edx
	movl	%edx, -36(%rbp)
	movss	-36(%rbp), %xmm0
	movl	%eax, %edi
	call	func_casting
	movl	-32(%rbp), %eax
	pxor	%xmm0, %xmm0
	cvtsi2ss	%eax, %xmm0
	movss	-24(%rbp), %xmm1
	cvttss2si	%xmm1, %eax
	movl	%eax, %edi
	call	func_casting
	movl	$0, %eax
	call	func_struct
	movl	$0, %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L5
	call	__stack_chk_fail
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.globl	func
	.type	func, @function
func:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	$10, -4(%rbp)
	movl	x(%rip), %edx
	movl	y(%rip), %eax
	addl	%eax, %edx
	movl	-4(%rbp), %eax
	addl	%eax, %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	movl	$10, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	func, .-func
	.globl	func_struct
	.type	func_struct, @function
func_struct:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$24, %edi
	call	malloc
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movl	$5, (%rax)
	movq	-16(%rbp), %rax
	movss	.LC1(%rip), %xmm0
	movss	%xmm0, 4(%rax)
	movq	-16(%rbp), %rax
	movb	$48, 8(%rax)
	movq	-16(%rbp), %rax
	movq	$0, 16(%rax)
	movl	$24, %edi
	call	malloc
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$10, (%rax)
	movq	-8(%rbp), %rax
	movb	$49, 8(%rax)
	movq	-8(%rbp), %rax
	movss	.LC2(%rip), %xmm0
	movss	%xmm0, 4(%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	free
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	func_struct, .-func_struct
	.globl	recursion
	.type	recursion, @function
recursion:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	cmpl	$0, -4(%rbp)
	je	.L13
	movl	-4(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %edi
	call	recursion
	jmp	.L10
.L13:
	nop
.L10:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	recursion, .-recursion
	.globl	func_static
	.type	func_static, @function
func_static:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	ans.3033(%rip), %edx
	movl	-4(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, ans.3033(%rip)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	func_static, .-func_static
	.globl	func_casting
	.type	func_casting, @function
func_casting:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movss	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	cvtsi2ss	-20(%rbp), %xmm0
	addss	-24(%rbp), %xmm0
	cvttss2si	%xmm0, %eax
	movl	%eax, -8(%rbp)
	pxor	%xmm0, %xmm0
	cvtsi2ss	-20(%rbp), %xmm0
	addss	-24(%rbp), %xmm0
	movss	%xmm0, -4(%rbp)
	movss	-24(%rbp), %xmm0
	cvttss2si	%xmm0, %edx
	movl	-8(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -20(%rbp)
	pxor	%xmm0, %xmm0
	cvtsi2ss	-20(%rbp), %xmm0
	addss	-4(%rbp), %xmm0
	movss	%xmm0, -24(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	func_casting, .-func_casting
	.globl	func_union
	.type	func_union, @function
func_union:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$4279881, -16(%rbp)
	movb	$65, -16(%rbp)
	movb	$66, -16(%rbp)
	leaq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	puts
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L19
	call	__stack_chk_fail
.L19:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	func_union, .-func_union
	.globl	func_enum
	.type	func_enum, @function
func_enum:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$1, e1(%rip)
	movl	$4, e2(%rip)
	movl	e1(%rip), %edx
	movl	e2(%rip), %eax
	addl	%edx, %eax
	movl	%eax, -4(%rbp)
	cmpl	$2, -4(%rbp)
	jle	.L23
	movl	$0, -4(%rbp)
	nop
.L23:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	func_enum, .-func_enum
	.section	.rodata
.LC3:
	.string	"INA"
	.text
	.globl	func_pointer
	.type	func_pointer, @function
func_pointer:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	$.LC3, -64(%rbp)
	leaq	-48(%rbp), %rax
	movq	%rax, -56(%rbp)
	movq	-64(%rbp), %rax
	addq	$1, %rax
	movl	%eax, -72(%rbp)
	movq	-56(%rbp), %rax
	addq	$4, %rax
	movl	%eax, -68(%rbp)
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L25
	call	__stack_chk_fail
.L25:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	func_pointer, .-func_pointer
	.local	ans.3033
	.comm	ans.3033,4,4
	.section	.rodata
	.align 4
.LC0:
	.long	1075838976
	.align 4
.LC1:
	.long	1069547520
	.align 4
.LC2:
	.long	1073741824
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
