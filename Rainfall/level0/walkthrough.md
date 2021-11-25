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
	   0x080