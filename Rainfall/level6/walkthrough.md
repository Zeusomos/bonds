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
       0x0804847f <+3>:	and    esp,0xfffffff0
       0x08048482 <+6>:	sub    esp,0x20
       0x08048485 <+9>:	mov    DWORD PTR [esp],0x40 			;argument for malloc
       0x0804848c <+16>:	call   0x8048350 <malloc@plt> 		;call malloc with arguments
       0x08048491 <+21>:	mov    DWORD PTR [esp+0x1c],eax 	;result of malloc to some value
       0x08048495 <+25>:	mov    DWORD PTR [esp],0x4  		;argument for malloc
       0x0804849c <+32>:	call   0x8048350 <malloc@plt>   	;call malloc with arguments
       0x080484a1 <+37>:	mov    DWORD PTR [esp+0x18],eax 	;result of malloc to some value
       0x080484a5 <+41>:	mov    edx,0x8048468 			;upload the beginning of the m function 
       0x080484aa <+46>:	mov    eax,DWORD PTR [esp+0x18] 	;something with variables, put address of m function to the variable
       0x080484ae <+50>:	mov    DWORD PTR [eax],edx 		;something with variables, put address of m funct