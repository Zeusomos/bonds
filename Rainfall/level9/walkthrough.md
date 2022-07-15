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
	End of as