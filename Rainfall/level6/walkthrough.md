# Preparations

Entering level6 home directory we see a level6 binary with sticky bit in access rights.
By objdumping the binary we see 3 functions: main, m and n:

	level6@RainFall:~$ objdump -D -M intel-mnemonic level6 > /tmp/dump

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
    Dump of assembler code for function main:
       0x0804847c <+0>:	push   ebp
       0x0804847d <+1>:	mov    ebp,esp
       0x0804847f <+3>:	and    esp,0xffff