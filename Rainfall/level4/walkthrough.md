
# Preparations

Entering level4 home directory we see a level4 binary with sticky bit in access rights.
By objdumping the binary we see 3 functions: main, n and p:

	level4@RainFall:~$ objdump -D -M intel-mnemonic level4 > /tmp/dump

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x080484a7 <+0>:	push   ebp
	   0x080484a8 <+1>:	mov    ebp,esp
	   0x080484aa <+3>:	and    esp,0xfffffff0
	   0x080484ad <+6>:	call   0x8048457 <n>
	   0x080484b2 <+11>:	leave
	   0x080484b3 <+12>:	ret

The only action after prologue is a call of the n function.
So main will look like:

	int main(void)
	{
		n();
	}

Let's look at the n function:

	(gdb) disass n
	Dump of assembler code for function n:
	   0x08048457 <+0>:	push   ebp
	   0x08048458 <+1>:	mov    ebp,esp
	   0x0804845a <+3>:	sub    esp,0x218
	   0x08048460 <+9>:	mov    eax,ds:0x8049804			;as last times we start to upload some FILE stream
	   0x08048465 <+14>:	mov    DWORD PTR [esp+0x8],eax		;argument for fgets
	   0x08048469 <+18>:	mov    DWORD PTR [esp+0x4],0x200	;argument for fgets
	   0x08048471 <+26>:	lea    eax,[ebp-0x208] 			;argument for fgets
	   0x08048477 <+32>:	mov    DWORD PTR [esp],eax 		;argument for fgets
	   0x0804847a <+35>:	call   0x8048350 <fgets@plt>		; call fgets with arguments placed on stack
	   0x0804847f <+40>:	lea    eax,[ebp-0x208]			;argument for fgets becomes argument for p function
	   0x08048485 <+46>:	mov    DWORD PTR [esp],eax		;argument for p function
	   0x08048488 <+49>:	call   0x8048444 <p>			; call p with arguments
	   0x0804848d <+54>:	mov    eax,ds:0x8049810 		;upload some FILE stream or some global
	   0x08048492 <+59>:	cmp    eax,0x1025544			;compare global and number
	   0x08048497 <+64>:	jne    0x80484a5 <n+78> 		;if not equal
	   0x08048499 <+66>:	mov    DWORD PTR [esp],0x8048590 	;argument for system
	   0x080484a0 <+73>:	call   0x8048360 <system@plt>		;call system with arguments
	   0x080484a5 <+78>:	leave
	   0x080484a6 <+79>:	ret

Let's look at the p function:

	(gdb) disass p
	Dump of assembler code for function p:
	   0x08048444 <+0>:	push   ebp
	   0x08048445 <+1>:	mov    ebp,esp
	   0x08048447 <+3>:	sub    esp,0x18
	   0x0804844a <+6>:	mov    eax,DWORD PTR [ebp+0x8] 		;preparation to printf
	   0x0804844d <+9>:	mov    DWORD PTR [esp],eax 		;argument for printf
	   0x08048450 <+12>:	call   0x8048340 <printf@plt>		;call printf with arguments
	   0x08048455 <+17>:	leave
	   0x08048456 <+18>:	ret

Let's see some insites by their addresses:

	(gdb) x/s 0x8049804
		0x8049804 <stdin@@GLIBC_2.0>:	 "\300", <incomplete sequence \375\267>
	(gdb) p/x $ebp-0x208
		$3 = 0xbffff4f0
	(gdb) p *0x08049810
		$5 = 0

This variable (global one) equals zero that means that it is static (in C all the static variables are 0 by declaration):

	(gdb) x/s 0x8049810
		0x8049810 <m>:	 ""
	(gdb) x/s 0x8048590
		0x8048590:	 "/bin/cat /home/user/level5/.pass"

All the strings and FILE streams are known now.\
We also need to get the buffer size that is sent to first `fgets` function. We look at the stack + make tests:

	Breakpoint 3, 0x08048488 in n ()
		(gdb) info frame
		Stack level 0, frame at 0xbffff700:
		 eip = 0x8048488 in n; saved eip **0x80484b2**
		 called by frame at 0xbffff710
		 Arglist at 0xbffff6f8, args:
		 Locals at 0xbffff6f8, Previous frame's sp is 0xbffff700
		 Saved registers:
		  ebp at 0xbffff6f8, eip at 0xbffff6fc

	Breakpoint 4, 0x0804847a in n ()
	(gdb) x/50wx $esp
		0xbffff4e0:	0xbffff4f0	0x00000200	0xb7fd1ac0	0xb7ff37d0
		0xbffff4f0:	0x00000000	0xb7e2b900	0x00000001	0xb7fef305
		0xbffff500:	0xbffff558	0xb7fde2d4	0xb7fde334	0x00000007
		0xbffff510:	0x00000000	0xb7fde000	0xb7fff53c	0xbffff558
		0xbffff520:	0x00000040	0x00000b80	0x00000000	0xb7fde714
		0xbffff530:	0x00000098	0x0000000b	0x00000000	0x00000000
		0xbffff540:	0x00000000	0x00000000	0x00000000	0x00000000
		0xbffff550:	0x00000000	0xb7fe765d	0xb7e3ebaf	0x08048285
		0xbffff560:	0x00000000	0x00000000	0x00000000	0xb7fe749f
		0xbffff570:	0xb7fd5000	0x00000002	0x08048240	0xb7fffc00
		0xbffff580:	0xb7ffeff4	0xb7ffeff4	0xb7e2fc30	0x00000001
		0xbffff590:	0xb7fdcb18	0xb7fe7cac	0x00000000	0x00000000
		0xbffff5a0:	0x00000000	0x00000000
	(gdb) c
		Continuing.
		AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

	Breakpoint 3, 0x08048488 in n ()
	(gdb) x/150wx $esp
		0xbffff4e0:	0xbffff4f0	0x00000200	0xb7fd1ac0	0xb7ff37d0
		0xbffff4f0:	0x41414141	0x41414141	0x41414141	0x41414141
		0xbffff500:	0x41414141	0x41414141	0x41414141	0x41414141
		0xbffff510:	0x41414141	0x41414141	0x41414141	0x41414141
		0xbffff520:	0x41414141	0x41414141	0x41414141	0x41414141