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
       0x080484ae <+50>:	mov    DWORD PTR [eax],edx 		;something with variables, put address of m function to the variable
       0x080484b0 <+52>:	mov    eax,DWORD PTR [ebp+0xc] 		;something with variables, argv is used
       0x080484b3 <+55>:	add    eax,0x4 				;something with variables, argv[1] is taken
       0x080484b6 <+58>:	mov    eax,DWORD PTR [eax] 		;arguments for strcpy
       0x080484b8 <+60>:	mov    edx,eax 				;arguments for strcpy
       0x080484ba <+62>:	mov    eax,DWORD PTR [esp+0x1c] 	;arguments for strcpy
       0x080484be <+66>:	mov    DWORD PTR [esp+0x4],edx 		;arguments for strcpy
       0x080484c2 <+70>:	mov    DWORD PTR [esp],eax 		;arguments for strcpy
       0x080484c5 <+73>:	call   0x8048340 <strcpy@plt>   	;call strcpy with arguments
       0x080484ca <+78>:	mov    eax,DWORD PTR [esp+0x18] 	;something with variables, deal with variable that we used before
       0x080484ce <+82>:	mov    eax,DWORD PTR [eax] 		;prepare to call functions that is a variable
       0x080484d0 <+84>:	call   eax 				;call function that lies in a variable
       0x080484d2 <+86>:	leave
       0x080484d3 <+87>:	ret

Something new!\
Let's look at the m function:

	(gdb) disass m
    Dump of assembler code for function m:
       0x08048468 <+0>:	push   ebp
       0x08048469 <+1>:	mov    ebp,esp
       0x0804846b <+3>:	sub    esp,0x18
       0x0804846e <+6>:	mov    DWORD PTR [esp],0x80485d1 		;argument for puts
       0x08048475 <+13>:	call   0x8048360 <puts@plt> 		;call puts with arguments
       0x0804847a <+18>:	leave
       0x0804847b <+19>:	ret

Let's look at the n function:

	(gdb) disass n
    Dump of assembler code for function n:
       0x08048454 <+0>:	push   ebp
       0x08048455 <+1>:	mov    ebp,esp
       0x08048457 <+3>:	sub    esp,0x18
       0x0804845a <+6>:	mov    DWORD PTR [esp],0x80485b0 		;argument for system
       0x08048461 <+13>:	call   0x8048370 <system@plt>   	;call system with arguments
       0x08048466 <+18>:	leave
       0x08048467 <+19>:	ret

Let's see some insites by their addresses:

	Breakpoint 10, 0x080484d0 in main ()
    (gdb) p/x $eax
        $5 = 0x8048468
    (gdb) x/s 0x80485b0
        $6 