# Preparations

Entering bonus0 home directory we see a bonus0 binary with sticky bit in access rights.
By objdumping the binary we see 3 functions main, pp and p:

	bonus0@RainFall:~$ objdump -D -M intel-mnemonic bonus0 > /tmp/dump_bon0

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x080485a4 <+0>:	push   ebp
	   0x080485a5 <+1>:	mov    ebp,esp
	   0x080485a7 <+3>:	and    esp,0xfffffff0
	   0x080485aa <+6>:	sub    esp,0x40
	   0x080485ad <+9>:	lea    eax,[esp+0x16]  		;buffer to pp
	   0x080485b1 <+13>:	mov    DWORD PTR [esp],eax 	;argument to pp
	   0x080485b4 <+16>:	call   0x804851e <pp>		;call pp
	   0x080485b9 <+21>:	lea    eax,[esp+0x16]		;argument to puts
	   0x080485bd <+25>:	mov    DWORD PTR [esp],eax	;argument to puts
	   0x080485c0 <+28>:	call   0x80483b0 <puts@plt>	;call puts
	   0x080485c5 <+33>:	mov    eax,0x0
	   0x080485ca <+38>:	leave
	   0x080485cb <+39>:	ret
	
We have buffer here, let's count its size:

	Breakpoint 2, 0x080485b4 in main ()
	(gdb) info frame
		Stack level 0, frame at 0xbffff710:
		 eip = 0x80485b4 in main; saved eip *0xb7e454d3*
		 Arglist at 0xbffff708, args:
		 Locals at 0xbffff708, Previous frame's sp is 0xbffff710
		 Saved registers:
		  ebp at 0xbffff708, eip at 0xbffff70c
	(gdb) x/20wx $esp
		0xbffff6c0:	0xbffff6d6	0x080498d8	0x00000001	0x0804835d
		0xbffff6d0:	0xb7fd13e4	0x00000014	0x080498d8	0x080485f1
		0xbffff6e0:	0xffffffff	0xb7e5edc6	0xb7fd0ff4	0xb7e5ee55
		0xbffff6f0:	0xb7fed280	0x00000000	0x080485d9	0xb7fd0ff4
		0xbffff700:	0x080485d0	0x00000000	0x00000000	*0xb7e454d3*
	(gdb) p/x $esp
		$7 = 0xbffff6c0
	(gdb) p/x $esp + 0x16
		$8 = 0xbffff6d6

Buffer starts in `0xbffff6d6` and `EIP` return address is in `0xbffff70c`:

	(gdb) p/x 0xbffff700 + 12
		$3 = 0xbffff70c

That is 54 bytes:
	
	(gdb) p 0xbffff70c - 0xbffff6d6
		$15 = 54
	
But the real buffer size is 42 bytes because after buffer we also have puts function before returning from main.\
Let's go to `pp` function:

	(gdb) disass pp
	Dump of assembler code for function pp:
	   0x0804851e <+0>:	push   ebp
	   0x0804851f <+1>:	mov    ebp,esp
	   0x08048521 <+3>:	push   edi
	   0x08048522 <+4>:	push   ebx
	   0x08048523 <+5>:	sub    esp,0x50
	   0x08048526 <+8>:	mov    DWORD PTR [esp+0x4],0x80486a0 	;argument to p
	   0x0804852e <+16>:	lea    eax,[ebp-0x30] 			;preparations to p
	   0x08048531 <+19>:	mov    DWORD PTR [esp],eax 		;argument to p
	   0x08048534 <+22>:	call   0x80484b4 <p>			;call p
	   0x08048539 <+27>:	mov    DWORD PTR [esp+0x4],0x80486a0 	;argument to p
	   0x08048541 <+35>:	lea    eax,[ebp-0x1c] 			;preparations to p
	   0x08048544 <+38>:	mov    DWORD PTR [esp],eax 		;argument to p
	   0x08048547 <+41>:	call   0x80484b4 <p>			;call p
	   0x0804854c <+46>:	lea    eax,[ebp-0x30] 			;preparations to strcpy
	   0x0804854f <+49>:	mov    DWORD PTR [esp+0x4],eax 		;argument to strcpy
	   0x08048553 <+53>:	mov    eax,DWORD PTR [ebp+0x8] 		;preparations to strcpy
	   0x08048556 <+56>:	mov    DWORD PTR [esp],eax 		;argument to strcpy
	   0x08048559 <+59>:	call   0x80483a0 <strcpy@plt>		;call strcpy
	   0x0804855e <+64>:	mov    ebx,0x80486a4			;preparations to strlen
	   0x08048563 <+69>:	mov    eax,DWORD PTR [ebp+0x8] 		;buffer is loaded coming as argument

	   That is strlen:
	   0x08048566 <+72>:	mov    DWORD PTR [ebp-0x3c],0xffffffff
	   0x0804856d <+79>:	mov    edx,eax
	   0x0804856f <+81>:	mov    eax,0x0
	   0x08048574 <+86>:	mov    ecx,DWORD PTR [ebp-0x3c]
	   0x08048577 <+89>:	mov    edi,edx
	   0x08048579 <+91>:	repnz scas al,BYTE PTR es:[edi]
	   0x0804857b <+93>:	mov    eax,ecx
	   0x0804857d <+95>:	not    eax
	   0x0804857f <+97>:	sub    eax,0x1
	   0x08048582 <+100>:	add    eax,DWORD PTR [ebp+0x8]
	   0x08048585 <+103>:	movzx  edx,WORD PTR [ebx]
	   0x08048588 <+106>:	mov    WORD PTR [eax],dx
	
	   0x0804858b <+109>:	lea    eax,[ebp-0x1c] 			;preparations to strcat
	   0x0804858e <+112>:	mov    DWORD PTR [esp+0x4],eax 		;argument to strcat
	   0x08048592 <+116>:	mov    eax,DWORD PTR [ebp+0x8] 		;preparations to strcat
	   0x08048595 <+119>:	mov    DWORD PTR [esp],eax 		;argument to strcat
	   0x08048598 <+122>:	call   0x8048390 <strcat@plt>		;call strcat
	   0x0804859d <+127>:	add    esp,0x50
	   0x080485a0 <+130>:	pop    ebx
	   0x080485a1 <+131>:	pop    edi
	   0x080485a2 <+132>:	pop    ebp
	   0x080485a3 <+133>:	ret

Let's see what we have under some addresses:

	(gdb) x/s 0x80486a0
		0x80486a0:	 " - "
	(gdb) x/s 0x80486a4
		0x80486a4:	 " "

And look how far `EIP` from pp lies:

	(gdb) info frame
		Stack level 0, frame at 0xbffff6c0:
		 eip = 0x8048523 in pp; saved eip *0x80485b9*
		 called by frame at 0xbffff710
		 Arglist at 0xbffff6b8, args:
		 Locals at 0xbffff6b8, Previous frame's sp is 0xbffff6c0
		 Saved registers:
		  ebp at 0xbffff6b8, eip at 0xbffff6bc
	(gdb) x/30wx $esp
		0xbffff6b0:	0xb7fd0ff4	0x00000000	0xbffff708	*0x080485b9*
		0xbffff6c0:	0xbffff6d6	0x080498d8	0x00000001	0x0804835d

And finally let's look at `p` function:

	(gdb) disass p
	Dump of assembler code for function p:
	   0x080484b4 <+0>:	push   ebp
	   0x080484b5 <+1>:	mov    ebp,esp
	   0x080484b7 <+3>:	sub    esp,0x1018
	=> 0x080484bd <+9>:	mov    eax,DWORD PTR [ebp+0xc] 		;argument coming to function
	   0x080484c0 <+12>:	mov    DWORD PTR [esp],eax 		;argument to puts
	   0x080484c3 <+15>:	call   0x80483b0 <puts@plt>		;call puts
	   0x080484c8 <+20>:	mov    DWORD PTR [esp+0x8],0x1000 	;argument to read, 4096 in dec
	   0x080484d0 <+28>:	lea    eax,[ebp-0x1008] 		;argument to read, buffer
	   0x080484d6 <+34>:	mov    DWORD PTR [esp+0x4],eax 		;preparations to read
	   0x080484da <+38>:	mov    DWORD PTR [esp],0x0 		;preparations to read, 0 as stream
	   0x080484e1 <+45>:	call   0x8048380 <read@plt>		;call read
	   0x080484e6 <+50>:	mov    DWORD PTR [esp+0x4],0xa 		;preparations to strchr,  10 or “\n” (10 nl)
	   0x080484ee <+58>:	lea    eax,[ebp-0x1008] 		;argument to strchr, buffer
	   0x080484f4 <+64>:	mov    DWORD PTR [esp],eax 		;preparations to strchr
	   0x080484f7 <+67>:	call   0x80483d0 <strchr@plt>		;call strchr
	   0x080484fc <+72>:	mov    BYTE PTR [eax],0x0 		;strchr result to null
	   0x080484ff <+75>:	lea    eax,[ebp-0x1008] 		