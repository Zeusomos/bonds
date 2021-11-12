# Preparations

Entering level0 home directory we see a level0 binary with sticky bit in access rights.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x08048ec0 <+0>:	push   ebp
	   0x08048ec1 <+1>:	mov    ebp,esp
	   0x08048ec3 <+3>:	and    es