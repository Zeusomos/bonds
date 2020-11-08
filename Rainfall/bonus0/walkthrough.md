# Preparations

Entering bonus0 home directory we see a bonus0 binary with sticky bit in access rights.
By objdumping the binary we see 3 functions main, pp and p:

	bonus0@RainFall:~$ objdump -D -M intel-mnemonic bonus0 > /tmp/dump_bon0

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x080485a4 <+0>:	push   ebp
	   0x080485a5 <+1>:	mov    ebp,esp
	   0x080485a7 <+3>:	and    esp,0xfffffff0
	   0x080485aa <+6>:	sub    esp,0x40
	   0x080485ad <+9>:	lea    eax,[esp+0x16]  		;buffer to pp
	   0x080485b1 <+13>:	mov    DWORD PTR [esp],eax 	;argument to pp
	   0x080485b4 <+16>:	call   0x804851e <pp>		;call pp
	   0x080485b9 <+21>:	lea    ea