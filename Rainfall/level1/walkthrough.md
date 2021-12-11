
# Preparations

Entering level1 home directory we see a level1 binary with sticky bit in access rights.
By objdumping the binary we see 2 functions: main and run:

	level1@RainFall:~$ objdump -D -M intel-mnemonic level1 > /tmp/dump

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x08048480 <+0>:	push   ebp
	   0x08048481 <+1>:	mov    ebp,esp
	   0x08048483 <+3>:	and    esp,0xfffffff0
	   0x08048486 <+6>:	sub    esp,0x50
	   0x08048489 <+9>:	lea    eax,[esp+0x10]		;preparations to gets
	   0x0804848d <+13>:	mov    DWORD PTR [esp],eax	;arguments for gets
	   0x08048490 <+16>:	call   0x8048340 <gets@plt>	;call gets
	=> 0x08048495 <+21>:	leave
	   0x08048496 <+22>:	ret

So main will look like:

	int	main(void)
	{
		char buffer[64];
		gets(buffer);
	}

Let's look in the run function:

	(gdb) disass run
	Dump of assembler code for function run:
	   0x08048444 <+0>:	push   ebp
	   0x08048445 <+1>:	mov    ebp,esp
	   0x08048447 <+3>:	sub    esp,0x18
	   0x0804844a <+6>:	mov    eax,ds:0x80497c0		;getting ready to fwrite, some FILE stream
	   0x0804844f <+11>:	mov    edx,eax			;getting ready to fwrite
	   0x08048451 <+13>:	mov    eax,0x8048570		;getting ready to fwrite
	   0x08048456 <+18>:	mov    DWORD PTR [esp+0xc],edx	;arguments for fwrite
	   0x0804845a <+22>:	mov    DWORD PTR [esp+0x8],0x13	;arguments for fwrite
	   0x08048462 <+30>:	mov    DWORD PTR [esp+0x4],0x1	;arguments for fwrite
	   0x0804846a <+38>:	mov    DWORD PTR [esp],eax	;arguments for fwrite
	   0x0804846d <+41>:	call   0x8048350 <fwrite@plt>	;call fwrite
	   0x08048472 <+46>:	mov    DWORD PTR [esp],0x8048584	;arguments for system
	   0x08048479 <+53>:	call   0x8048360 <system@plt>		;call system
	   0x0804847e <+58>:	leave
	   0x0804847f <+59>:	ret

Let's see some insites by their addresses:

	(gdb) x/20c 0x8048570
		0x8048570:	71 'G'	111 'o'	111 'o'	100 'd'	46 '.'	46 '.'	46 '.'	32 ' '
		0x8048578:	87 'W'	97 'a'	105 'i'	116 't'	32 ' '	119 'w'	104 'h'	97 'a'
		0x8048580:	116 't'	63 '?'	10 '\n'	0 '\000'
	(gdb) x/16c 0x80497c0
		0x80497c0 <stdout@@GLIBC_2.0>:	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'
