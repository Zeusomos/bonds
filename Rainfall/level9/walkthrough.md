# Preparations

Entering level9 home directory we see a level9 binary with sticky bit in access rights.
By objdumping the binary we see only main function with classes:

	level9@RainFall:~$ objdump -D -M intel-mnemonic level9 > /tmp/dump9

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x080485f4 <+0>:	push   ebp
	   0x080485f5 <+1>:	mov    ebp,esp
	   0x080485f7 <+3>:	push   ebx
	   0x080485f8 <+4>:	and    esp,0xfffffff0
	   0x080485fb <+7>:	sub    esp,0x20
	   0x080485fe <+10>:	cmp    DWORD PTR [ebp+0x8],0x1			;check arguments
	   0x08048602 <+14>:	jg     0x8048610 <main+28>			;jump if not according to condition (first operand is more than second operand)
	   0x08048604 <+16>:	mov    DWORD PTR [esp],0x1			;argument for exit
	   0x0804860b <+23>:	call   0x80484f0 <_exit@plt>			;call exit
	   0x08048610 <+28>:	mov    DWORD PTR [esp],0x6c			;preparation to calling the class
	   0x08048617 <+35>:	call   0x8048530 <_Znwj@plt>			;calling the class
	   0x0804861c <+40>:	mov    ebx,eax					;preparations to calling the method
	   0x0804861e <+42>:	mov    DWORD PTR [esp+0x4],0x5			;argument for calling the method
	   0x08048626 <+50>:	mov    DWORD PTR [esp],ebx			;preparations to calling the method
	   0x08048629 <+53>:	call   0x80486f6 <_ZN1NC2Ei>			;calling the method new
	   0x0804862e <+58>:	mov    DWORD PTR [esp+0x1c],ebx			;result to the variable
	   0x08048632 <+62>:	mov    DWORD