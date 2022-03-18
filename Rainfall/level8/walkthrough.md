
# Preparations

Entering level8 home directory we see a level8 binary with sticky bit in access rights.
By objdumping the binary we see only main function:

	level8@RainFall:~$ objdump -D -M intel-mnemonic level8 > /tmp/dump

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x08048564 <+0>:	push   ebp
	   0x08048565 <+1>:	mov    ebp,esp
	   0x08048567 <+3>:	push   edi
	   0x08048568 <+4>:	push   esi
	   0x08048569 <+5>:	and    esp,0xfffffff0
	   0x0804856c <+8>:	sub    esp,0xa0
	   0x08048572 <+14>:	jmp    0x8048575 <main+17> 		;inside two nop it is a cycle
	   0x08048574 <+16>:	nop
	   0x08048575 <+17>:	mov    ecx,DWORD PTR ds:0x8049ab0 	;preparations to printf
	   0x0804857b <+23>:	mov    edx,DWORD PTR ds:0x8049aac 	;preparations to printf
	   0x08048581 <+29>:	mov    eax,0x8048810 			;preparations to printf
	   0x08048586 <+34>:	mov    DWORD PTR [esp+0x8],ecx 		;argument to print
	   0x0804858a <+38>:	mov    DWORD PTR [esp+0x4],edx 		;argument to print
	   0x0804858e <+42>:	mov    DWORD PTR [esp],eax 		;argument to print
	   0x08048591 <+45>:	call   0x8048410 <printf@plt>		;call printf
	   0x08048596 <+50>:	mov    eax,ds:0x8049a80 		;preparations to fgets
	   0x0804859b <+55>:	mov    DWORD PTR [esp+0x8],eax 		;preparations to fgets
	   0x0804859f <+59>:	mov    DWORD PTR [esp+0x4],0x80 	;argument to fgets
	   0x080485a7 <+67>:	lea    eax,[esp+0x20] 			;argument to fgets