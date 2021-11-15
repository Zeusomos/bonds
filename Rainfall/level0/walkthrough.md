# Preparations

Entering level0 home directory we see a level0 binary with sticky bit in access rights.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x08048ec0 <+0>:	push   ebp
	   0x08048ec1 <+1>:	mov    ebp,esp
	   0x08048ec3 <+3>:	and    esp,0xfffffff0
	   0x08048ec6 <+6>:	sub    esp,0x20
	   0x08048ec9 <+9>:	mov    eax,DWORD PTR [ebp+0xc]		;some variables
	   0x08048ecc <+12>:	add    eax,0x4				;some variables
	   0x08048ecf <+15>:	mov    eax