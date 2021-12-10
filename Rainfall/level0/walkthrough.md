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
	   0x08048ecf <+15>:	mov    eax,DWORD PTR [eax]		;getting ready to atoi
	   0x08048ed1 <+17>:	mov    DWORD PTR [esp],eax		;arguments for atoi
	   0x08048ed4 <+20>:	call   0x8049710 <atoi>			;call atoi
	   0x08048ed9 <+25>:	cmp    eax,0x1a7			;compare variable with 423
	   0x08048ede <+30>:	jne    0x8048f58 <main+152>		;if not equal - jump to 0x8048f58 instruction
	   0x08048ee0 <+32>:	mov    DWORD PTR [esp],0x80c5348	;preparation to strdup
	   0x08048ee7 <+39>:	call   0x8050bf0 <strdup>		;call strdup
	   0x08048eec <+44>:	mov    DWORD PTR [esp+0x10],eax		;put result of strdup to variable
	   0x08048ef0 <+48>:	mov    DWORD PTR [esp+0x14],0x0		;put NULL to variable
	   0x08048ef8 <+56>:	call   0x8054680 <getegid>		;call getegid
	   0x08048efd <+61>:	mov    DWORD PTR [esp+0x1c],eax		;put result of getgid to variable
	   0x08048f01 <+65>:	call   0x8054670 <geteuid>		;call geteuid
	   0x08048f06 <+70>:	mov    DWORD PTR [esp+0x18],eax		;put result of getuid to variable
	   0x08048f0a <+74>:	mov    eax,DWORD PTR [esp+0x1c]		;preparation to setresgid
	   0x08048f0e <+78>:	mov    DWORD PTR [esp+0x8],eax		;arguments for setresgid
	   0x08048f12 <+82>:	mov    eax,DWORD PTR [esp+0x1c]		;preparation to setresgid
	   0x08048f16 <+86>:	mov    DWORD PTR [esp+0x4],eax		;arguments for setresgid
	   0x08048f1a <+90>:	mov    eax,DWORD PTR [esp+0x1c]		;preparation to setresgid
	   0x08048f1e <+94>:	mov    DWORD PTR [esp],eax		;arguments for setresgid
	   0x08048f21 <+97>:	call   0x8054700 <setresgid>		;call setresgid
	   0x08048f26 <+102>:	mov    eax,DWORD PTR [esp+0x18]		;preparation to setresuid
	   0x08048f2a <+106>:	mov    DWORD PTR [esp+0x8],eax		;arguments for setresuid
	   0x08048f2e <+110>:	mov    eax,DWORD PTR [esp+0x18]		;preparation to setresuid
	   0x08048f32 <+114>:	mov    DWORD PTR [esp+0x4],eax		;arguments for setresuid
	   0x08048f36 <+118>:	mov    eax,DWORD PTR [esp+0x18]		;preparation to setresuid
	   0x08048f3a <+122>:	mov    DWORD PTR [esp],eax		;arguments for setresuid
	   0x08048f3d <+125>:	call   0x8054690 <setresuid>		;call setresuid
	   0x08048f42 <+130>:	lea    eax,[esp+0x10]			;preparation to execv
	   0x08048f46 <+134>:	mov    DWORD PTR [esp+0x4],eax		;arguments for execv
	   0x08048f4a <+138>:	mov    DWORD PTR [esp],0x80c5348	;arguments for execv
	   0x08048f51 <+145>:	call   0x8054640 <execv>		;call execve
	   0x08048f56 <+150>:	jmp    0x8048f80 <main+192>
	   0x08048f58 <+152>:	mov    eax,ds:0x80ee170			;preparation to fwrite
	   0x08048f5d <+157>:	mov    edx,eax				;preparation to fwrite
	   0x08048f5f <+159>:	mov    eax,0x80c5350			;preparation to fwrite
	   0x08048f64 <+164>:	mov    DWORD PTR [esp+0xc],edx		;arguments for fwrite
	   0x08048f68 <+168>:	mov    DWORD PTR [esp+0x8],0x5		;arguments for fwrite
	   0x08048f70 <+176>:	mov    DWORD PTR [esp+0x4],0x1		;arguments for fwrite
	   0x08048f78 <+184>:	mov    DWORD PTR [esp],eax		;arguments for fwrite
	   0x08048f7b <+187>:	call   0x804a230 <fwrite>		;call fwrite
	   0x08048f80 <+192>:	mov    eax,0x0
	   0x08048f85 <+197>:	leave
	   0x08048f86 <+198>:	ret

Let's see some insites by their addresses:

	(gdb) x/8cb 0x80c5348
		0x80c5348:	47 '/'	98 'b'	105 'i'	110 'n'	47 '/'	115 's'	104 'h'	0 '\000'
	(gdb) x/16c 0x80ee170
		0x80ee170 <stderr>:	-96 '\240'	-25 '\347'	14 '\016'	8 '\b'	64 '@'	-24 '\350'	14 '\016'	8 '\b'
	(gdb) x/63c 0x80c5350
		0x80c5350:	78 'N'	111 'o'	32 ' '	33 '!'	10 '\n'	0 '\000'	70 'F'	65 'A'
		0x80c5358:	84 'T'	65 'A'	76 'L'	58 ':'	32 ' '	107 'k'	101 'e'	114 'r'
		0x80c5360:	110 'n'	101 'e'	108 'l'	32 ' '	116 't'	111 'o'	111 'o'	32 ' '

Numbers in hex are transferred to decimal with the help of programming calculator and finally we write the code in C.

# Exploitation

If `atoi` result equals 423 - we enter conditiol where execv is called. Therefore:

	./level0 423

# Result

	level0@RainFall:~$ ./level0 423
	$ whoami
	level1
	$ cat /home/user/level1/.pass
	1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
