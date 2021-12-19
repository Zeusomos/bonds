
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