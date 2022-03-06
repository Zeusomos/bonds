
# Preparations

Entering level7 home directory we see a level7 binary with sticky bit in access rights.
By objdumping the binary we see 2 functions: main and m:

	level7@RainFall:~$ objdump -D -M intel-mnemonic level7 > /tmp/dump

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x08048521 <+0>:	push   ebp
	   0x08048522 <+1>:	mov    ebp,esp
	   0x08048524 <+3>:	and    esp,0xfffffff0
	   0x08048527 <+6>:	sub    esp,0x20
	   0x0804852a <+9>:	mov    DWORD PTR [esp],0x8		;argument for malloc
	   0x08048531 <+16>:	call   0x80483f0 <malloc@plt>		;call malloc with arguments
	   0x08048536 <+21>:	mov    DWORD PTR [esp+0x1c],eax 	;malloc result saved to some variable
	   0x0804853a <+25>:	mov    eax,DWORD PTR [esp+0x1c] 	;getting ready with a variable
	   0x0804853e <+29>:	mov    DWORD PTR [eax],0x1 		;1st index in the string
	   0x08048544 <+35>:	mov    DWORD PTR [esp],0x8 		;argument for malloc
	   0x0804854b <+42>:	call   0x80483f0 <malloc@plt>		;call malloc with arguments
	   0x08048550 <+47>:	mov    edx,eax
	   0x08048552 <+49>:	mov    eax,DWORD PTR [esp+0x1c] 	;malloc result saved to the variable
	   0x08048556 <+53>:	mov    DWORD PTR [eax+0x4],edx 		;getting ready with a variable
	   0x08048559 <+56>:	mov    DWORD PTR [esp],0x8 		;argument for malloc
	   0x08048560 <+63>:	call   0x80483f0 <malloc@plt>		;call malloc with arguments
	   0x08048565 <+68>:	mov    DWORD PTR [esp+0x18],eax 	;malloc result saved to a variable
	   0x08048569 <+72>:	mov    eax,DWORD PTR [esp+0x18] 	;getting ready with some variable
	   0x0804856d <+76>:	mov    DWORD PTR [eax],0x2 		;2nd index in a string variable
	   0x08048573 <+82>:	mov    DWORD PTR [esp],0x8 		;argument to malloc
	   0x0804857a <+89>:	call   0x80483f0 <malloc@plt>		;call malloc with arguments
	   0x0804857f <+94>:	mov    edx,eax
	   0x08048581 <+96>:	mov    eax,DWORD PTR [esp+0x18] 	;результат malloc в переменную
	   0x08048585 <+100>:	mov    DWORD PTR [eax+0x4],edx  	;getting ready with some variable
	   0x08048588 <+103>:	mov    eax,DWORD PTR [ebp+0xc] 		;argv array
	   0x0804858b <+106>:	add    eax,0x4 ;аргумент argv[1]	;1st argv string
	   0x0804858e <+109>:	mov    eax,DWORD PTR [eax]		;getting ready to strcpy
	   0x08048590 <+111>:	mov    edx,eax				;getting ready to strcpy
	   0x08048592 <+113>:	mov    eax,DWORD PTR [esp+0x1c] 	;argument for strcpy
	   0x08048596 <+117>:	mov    eax,DWORD PTR [eax+0x4] 		;argument for strcpy
	   0x08048599 <+120>:	mov    DWORD PTR [esp+0x4],edx 		;argument for strcpy
	   0x0804859d <+124>:	mov    DWORD PTR [esp],eax 		;argument for strcpy
	   0x080485a0 <+127>:	call   0x80483e0 <strcpy@plt>		;call strcpy with arguments
	   0x080485a5 <+132>:	mov    eax,DWORD PTR [ebp+0xc] 		;argv array
	   0x080485a8 <+135>:	add    eax,0x8 				;2nd string from argv
	   0x080485ab <+138>:	mov    eax,DWORD PTR [eax]		;getting ready to strcpy
	   0x080485ad <+140>:	mov    edx,eax				;getting ready to strcpy
	   0x080485af <+142>:	mov    eax,DWORD PTR [esp+0x18] 	;argument for strcpy
	   0x080485b3 <+146>:	mov    eax,DWORD PTR [eax+0x4] 		;argument for strcpy
	   0x080485b6 <+149>:	mov    DWORD PTR [esp+0x4],edx 		;argument for strcpy
	   0x080485ba <+153>:	mov    DWORD PTR [esp],eax 		;argument for strcpy
	   0x080485bd <+156>:	call   0x80483e0 <strcpy@plt>		;call strcpy with arguments
	   0x080485c2 <+161>:	mov    edx,0x80486e9 			;getting ready to fopen
	   0x080485c7 <+166>:	mov    eax,0x80486eb 			;getting ready to fopen
	   0x080485cc <+171>:	mov    DWORD PTR [esp+0x4],edx 		;argument for fopen
	   0x080485d0 <+175>:	mov    DWORD PTR [esp],eax  		;argument for fopen
	   0x080485d3 <+178>:	call   0x8048430 <fopen@plt>		;call fopen with arguments
	   0x080485d8 <+183>:	mov    DWORD PTR [esp+0x8],eax 		;fopen result saved in a variable
	   0x080485dc <+187>:	mov    DWORD PTR [esp+0x4],0x44 	;arguments for fgets
	   0x080485e4 <+195>:	mov    DWORD PTR [esp],0x8049960 	;arguments for fgets
	   0x080485eb <+202>:	call   0x80483c0 <fgets@plt>		;call fgets with arguments
	   0x080485f0 <+207>:	mov    DWORD PTR [esp],0x8048703 	;arguments for puts
	   0x080485f7 <+214>:	call   0x8048400 <puts@plt>		;call puts with arguments
	   0x080485fc <+219>:	mov    eax,0x0 				;should be return(0)
	   0x08048601 <+224>:	leave
	   0x08048602 <+225>:	ret

Something new!\
Let's look at the m function:

	(gdb) disass m
	Dump of assembler code for function m:
	   0x080484f4 <+0>:	push   ebp
	   0x080484f5 <+1>:	mov    ebp,esp
	   0x080484f7 <+3>:	sub    esp,0x18
	   0x080484fa <+6>:	mov    DWORD PTR [esp],0x0		;argument to time
	   0x08048501 <+13>:	call   0x80483d0 <time@plt>		;call time with arguments
	   0x08048506 <+18>:	mov    edx,0x80486e0			;getting ready to printf
	   0x0804850b <+23>:	mov    DWORD PTR [esp+0x8],eax		;argument to prinf
	   0x0804850f <+27>:	mov    DWORD PTR [esp+0x4],0x8049960	;argument to prinf
	   0x08048517 <+35>:	mov    DWORD PTR [esp],edx		;argument to prinf
	   0x0804851a <+38>:	call   0x80483b0 <printf@plt>		;call printf with arguments
	   0x0804851f <+43>:	leave
	   0x08048520 <+44>:	ret

Let's see some insites by their addresses:

	(gdb) x/s 0x08049960
		0x8049960 <c>:	 ""

Seems like it is a global variable, let's check:

	Disassembly of section .bss:
		08049960 <c>:
		...

Yap. Continuing:

    (gdb) x/s 0x8048703
		0x8048703:	 "~~"