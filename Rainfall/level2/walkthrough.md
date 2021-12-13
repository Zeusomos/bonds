
# Preparations

Entering level2 home directory we see a level2 binary with sticky bit in access rights.
By objdumping the binary we see 2 functions: main and p:

	level2@RainFall:~$ objdump -D -M intel-mnemonic level2 > /tmp/dump

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x0804853f <+0>:	push   ebp
	   0x08048540 <+1>:	mov    ebp,esp
	   0x08048542 <+3>:	and    esp,0xfffffff0
	   0x08048545 <+6>:	call   0x80484d4 <p>
	   0x0804854a <+11>:	leave
	   0x0804854b <+12>:	ret

The only action after prologue is a call of the p function.
So main will look like:

	int main(void)
	{
		p();
	}

Let's look in the p function:

	(gdb) disass p
	Dump of assembler code for function p:
	   0x080484d4 <+0>:	push   ebp
	   0x080484d5 <+1>:	mov    ebp,esp
	   0x080484d7 <+3>:	sub    esp,0x68
	   0x080484da <+6>:	mov    eax,ds:0x8049860			;getting ready to fflush, some FILE stream
	   0x080484df <+11>:	mov    DWORD PTR [esp],eax		;arguments for fflush
	   0x080484e2 <+14>:	call   0x80483b0 <fflush@plt>		;call fflush
	   0x080484e7 <+19>:	lea    eax,[ebp-0x4c]			;getting ready to gets
	   0x080484ea <+22>:	mov    DWORD PTR [esp],eax		;arguments for gets
	   0x080484ed <+25>:	call   0x80483c0 <gets@plt>		;call gets
	   0x080484f2 <+30>:	mov    eax,DWORD PTR [ebp+0x4]		;something new, occured to be __builtin_return_address function
	   0x080484f5 <+33>:	mov    DWORD PTR [ebp-0xc],eax		;something new, occured to be __builtin_return_address function result put to a variable
	   0x080484f8 <+36>:	mov    eax,DWORD PTR [ebp-0xc]		;something new, occured to be __builtin_return_address function result variable that starts to be used
	   0x080484fb <+39>:	and    eax,0xb0000000			;bitwise AND operator
	   0x08048500 <+44>:	cmp    eax,0xb0000000			;comparison with 0xb0000000
	   0x08048505 <+49>:	jne    0x8048527 <p+83>			;if not equal than directly to puts function preparation
	   0x08048507 <+51>:	mov    eax,0x8048620			;getting ready to printf
	   0x0804850c <+56>:	mov    edx,DWORD PTR [ebp-0xc]		;getting ready to printf
	   0x0804850f <+59>:	mov    DWORD PTR [esp+0x4],edx		;arguments to print
	   0x08048513 <+63>:	mov    DWORD PTR [esp],eax		;arguments to print
	   0x08048516 <+66>:	call   0x80483a0 <printf@plt>		;call printf
	   0x0804851b <+71>:	mov    DWORD PTR [esp],0x1		;arguments to exit
	   0x08048522 <+78>:	call   0x80483d0 <_exit@plt>		;call exit
	   0x08048527 <+83>:	lea    eax,[ebp-0x4c]			;preparation to puts
	   0x0804852a <+86>:	mov    DWORD PTR [esp],eax		;arguments to puts
	   0x0804852d <+89>:	call   0x80483f0 <puts@plt>		;call puts
	   0x08048532 <+94>:	lea    eax,[ebp-0x4c]			;preparation to strdup
	   0x08048535 <+97>:	mov    DWORD PTR [esp],eax		;arguments to strdup
	   0x08048538 <+100>:	call   0x80483e0 <strdup@plt>		;call strdup
	   0x0804853d <+105>:	leave
	   0x0804853e <+106>:	ret

Let's see some insites by their addresses:

	(gdb) x/16cb 0x8049860
		0x8049860 <stdout@@GLIBC_2.0>:	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'
		0x8049868 <dtor_idx.6161>:	0 '\000'	0 '\000'	0 '\000'	0 '\000'	Cannot access memory at address 0x804986c

	Breakpoint 2, 0x080484e7 in p ()
	(gdb) i r ebp
		ebp            0xbffff6f8	0xbffff6f8
	(gdb) p/x 0xbffff6f8 - 0x4c
		$2 = 0xbffff6ac
	(gdb) x/16x 0xbffff6ac
		0xbffff6ac:	0x00000001	0xbffff8c1	0x0000002f	0xbffff70c
		0xbffff6bc:	0xb7fd0ff4	0x08048550	0x08049828	0x00000001
		0xbffff6cc:	0x08048381	0xb7fd13e4	0x00000014	0x08049828
		0xbffff6dc:	0x08048571	0xffffffff	0xb7e5edc6	0xb7fd0ff4
	(gdb) info frame
		Stack level 0, frame at 0xbffff700:
		 eip = 0x80484e7 in p; saved eip **0x804854a**
		 called by frame at 0xbffff710
		 Arglist at 0xbffff6f8, args:
		 Locals at 0xbffff6f8, Previous frame's sp is 0xbffff700
		 Saved registers:
		  ebp at 0xbffff6f8, eip at 0xbffff6fc

There is some rubbish before we put value to `$ebp - 0x4c` variable. But after:

	0x080484ed <+25>:	call   0x80483c0 <gets@plt>
	0x080484f2 <+30>:	mov    eax,DWORD PTR [ebp+0x4]
	0x080484f5 <+33>:	mov    DWORD PTR [ebp-0xc],eax

	(gdb) p $ebp + 0x4
		$1 = (void *) 0xbffff6fc
	(gdb) p/x *0xbffff6fc
		$3 = 0x804854a
