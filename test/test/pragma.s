	.file	"pragma.cpp"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB0:
	.section	.text.startup,"ax",@progbits
.LHOTB0:
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	movl	$28, %eax
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE0:
	.section	.text.startup
.LHOTE0:
	.ident	"GCC: (GNU) 4.9.2 20141101 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
