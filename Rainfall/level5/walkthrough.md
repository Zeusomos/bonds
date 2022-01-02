
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