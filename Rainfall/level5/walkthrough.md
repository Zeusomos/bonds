
# Preparations

Entering level5 home directory we see a level5 binary with sticky bit in access rights.
By objdumping the binary we see 3 functions: main, n and o:

	level5@RainFall:~$ objdump -D -M intel-mnemonic level5 > /tmp/dump

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x08048504 <+0>:	push   ebp
	   0x08048505 <+1>:	mov    ebp,esp
	   0x08048507 <+3>:	and    esp,0xfffffff0
	   0x0804850a <+6>:	call   0x80484c2 <n>
	   0x0804850f <+11>:	leave
	   0x08048510 <+12>:	ret

The only action after prologue is a call of the n function.
So main will look like:

	int main(void)
	{
		n();
	}

Let's look at the n function:

	(gdb) disass n
	Dump of assembler code for function n:
	   0x080484c2 <+0>:	push   ebp
	   0x080484c3 <+1>:	mov    ebp,esp
	   0x080484c5 <+3>:	sub    esp,0x218
	   0x080484cb <+9>:	mov    eax,ds:0x8049848 		;upload some FILE stream
	   0x080484d0 <+14>:	mov    DWORD PTR [esp+0x8],eax 		;argument for fgets
	   0x080484d4 <+18>:	mov    DWORD PTR [esp+0x4],0x200 	;argument for fgets
	   0x080484dc <+26>:	lea    eax,[ebp-0x208] 			;argument for fgets
	   0x080484e2 <+32>:	mov    DWORD PTR [esp],eax 		;argument for fgets
	   0x080484e5 <+35>:	call   0x80483a0 <fgets@plt>		; call fgets with arguments
	   0x080484ea <+40>:	lea    eax,[ebp-0x208] 			;preparation to printf
	   0x080484f0 <+46>:	mov    DWORD PTR [esp],eax 		;argument for printf
	   0x080484f3 <+49>:	call   0x8048380 <printf@plt>		;call printf with arguments
	   0x080484f8 <+54>:	mov    DWORD PTR [esp],0x1 		;argument 1 for exit
	   0x080484ff <+61>:	call   0x80483d0 <exit@plt>		;call exit with one

Let's look at the o function:

	(gdb) disass o
	Dump of assembler code for function o:
	   0x080484a4 <+0>:	push   ebp
	   0x080484a5 <+1>:	mov    ebp,esp
	   0x080484a7 <+3>:	sub    esp,0x18
	   0x080484aa <+6>:	mov    DWORD PTR [esp],0x80485f0 	;argument for system
	   0x080484b1 <+13>:	call   0x80483b0 <system@plt>		;call system with arguments
	   0x080484b6 <+18>:	mov    DWORD PTR [esp],0x1 		;argument 1 for exit
	   0x080484bd <+25>:	call   0x8048390 <_exit@plt>		;call exit with one

Let's see some insites by their addresses:

	(gdb) x/s 0x8049848
		0x8049848 <stdin@@GLIBC_2.0>:	 "\300", <incomplete sequence \375\267>
	(gdb) x/s 0x80485f0
		0x80485f0:	 "/bin/sh"

All the strings and FILE streams are known now.\
We also need to get the buffer size that is sent to first `fgets` function. We look at the stack + make tests:

	Breakpoint 2, 0x080484cb in n ()
	(gdb) info frame
		Stack level 0, frame at 0xbffff700:
		 eip = 0x80484cb in n; saved eip 0x804850f
		 called by frame at 0xbffff710
		 Arglist at 0xbffff6f8, args:
		 Locals at 0xbffff6f8, Previous frame's sp is 0xbffff700
		 Saved registers:
		  ebp at 0xbffff6f8, eip at 0xbffff6fc
	(gdb) x/150wx $esp
		0xbffff4e0:	0xb7fde612	0xb7ffeff4	0xbffff6a8	0xb7ff37d0
		0xbffff4f0:	0x00000000	0xb7e2b900	0x00000001	0xb7fef305
		0xbffff500:	0xbffff558	0xb7fde2d4	0xb7fde334	0x00000007
		0xbffff510:	0x00000000	0xb7fde000	0xb7fff53c	0xbffff558
		0xbffff520:	0x00000040	0x00000b80	0x00000000	0xb7fde714
		0xbffff530:	0x00000098	0x0000000b	0x00000000	0x00000000
		0xbffff540:	0x00000000	0x00000000	0x00000000	0x00000000
		0xbffff550:	0x00000000	0xb7fe765d	0xb7e3ebaf	0x080482ab
		0xbffff560:	0x00000000	0x00000000	0x00000000	0xb7fe749f
		0xbffff570:	0xb7fd5000	0x00000002	0x08048260	0xb7fffc00
		0xbffff580:	0xb7ffeff4	0xb7ffeff4	0xb7e2fc30	0x00000001
		0xbffff590:	0xb7fdcb18	0xb7fe7cac	0x00000000	0x00000000
		0xbffff5a0:	0x00000000	0x00000000	0x00000000	0x00000000
		0xbffff5b0:	0x00000000	0xb7fdcb18	0x00000000	0x00000000
		0xbffff5c0:	0x00000000	0x00000003	0xf63d4e2e	0x000003f3
		0xbffff5d0:	0x00000000	0xb7e38938	0xb7fffe78	0xb7ff9d5c
		0xbffff5e0:	0xb7e2fe38	0x00000000	0x00000026	0xb7e5d58d
		0xbffff5f0:	0x00000000	0x00000000	0x00000001	0x000008b0
		0xbffff600:	0xb7fdcb48	0xb7fdc858	0x08048299	0xb7e39158
		0xbffff610:	0x08048220	0x00000001	0xb7ec36c0	0xb7ec38ee
		0xbffff620:	0xbffff658	0xb7ffeff4	0xb7fffad0	0xbffff744
		0xbffff630:	0xbffff700	0xb7fe7ed9	0xbffff6e0	0x08048220
		0xbffff640:	0xbffff6c8	0xb7fffa74	0x00000000	0xb7fdcb48
		0xbffff650:	0x00000001	0x00000000	0x00000001	0xb7fff918
		0xbffff660:	0x00000000	0x00000000	0x00000000	0xb7fd0ff4
		0xbffff670:	0xbffff6be	0xbffff6bf	0x00000001	0xb7ec3c49
		0xbffff680:	0xbffff6bf	0xbffff6be	0x00000000	0xb7ff3fec
		0xbffff690:	0xbffff744	0x00000000	0x00000000	0xb7e5ec73
		0xbffff6a0:	0x08048299	0x00000000	0x00c30000	0x00000001
		0xbffff6b0:	0xbffff8c1	0x0000002f	0xbffff70c	0xb7fd0ff4
		0xbffff6c0:	0x08048520	0x08049818	0x00000001	0x0804835d
		0xbffff6d0:	0xb7fd13e4	0x00000014	0x08049818	0x08048541
		0xbffff6e0:	0xffffffff	0xb7e5edc6	0xb7fd0ff4	0xb7e5ee55
		0xbffff6f0:	0xb7fed280	0x00000000	0xbffff708	0x0804850f
		0xbffff700:	0x08048520	0x00000000	0x00000000	0xb7e454d3
		0xbffff710:	0x00000001	0xbffff7a4	0xbffff7ac	0xb7fdc858
		0xbffff720:	0x00000000	0xbffff71c	0xbffff7ac	0x00000000
		0xbffff730:	0x08048250	0xb7fd0ff4

		Breakpoint 3, 0x080484e5 in n ()
		(gdb) c
			Continuing.
			AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa

		Breakpoint 4, 0x080484f3 in n ()
		(gdb) x/150wx $esp
			0xbffff4e0:	0xbffff4f0	0x00000200	0xb7fd1ac0	0xb7ff37d0
			0xbffff4f0:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff500:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff510:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff520:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff530:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff540:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff550:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff560:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff570:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff580:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff590:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff5a0:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff5b0:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff5c0:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff5d0:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff5e0:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff5f0:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff600:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff610:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff620:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff630:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff640:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff650:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff660:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff670:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff680:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff690:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff6a0:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff6b0:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff6c0:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff6d0:	0x41414141	0x41414141	0x41414141	0x41414141
			0xbffff6e0:	0x41414141	0x41414141	0x41414141	0x00414141
			0xbffff6f0:	0xb7fed280	0x00000000	0xbffff708	0x0804850f
			0xbffff700:	0x08048520	0x00000000	0x00000000	0xb7e454d3
			0xbffff710:	0x00000001	0xbffff7a4	0xbffff7ac	0xb7fdc858
			0xbffff720:	0x00000000	0xbffff71c	0xbffff7ac	0x00000000
			0xbffff730:	0x08048250	0xb7fd0ff4

It is impossible to overwrite buffer because fgets takes definite number of symbols from stdin and all the other leaves there. From the address of the last found letter we substract the beginning of the buffer and get 511 byte + \0 = 512 byte.

	(gdb) p 0xbffff6f0 - 0xbffff4f0
		$5 = 512

Numbers in hex are transferred to decimal with the help of programming calculator and finally we write the code in C.\

# Exploitation

We need to call o function where system with string "/bin/sh" is called. It is not called now but after printf we have exit that we can replace with the address of the `o` function. It's address is:

	(gdb) disass o
	Dump of assembler code for function o:
	   0x080484a4 <+0>:	push   ebp

On this level we deal with format string exploits in `printf(buffer)`. It was difficult for understanding.\