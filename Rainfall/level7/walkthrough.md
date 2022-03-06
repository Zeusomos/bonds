
# Preparations

Entering level7 home directory we see a level7 binary with sticky bit in access rights.
By objdumping the binary we see 2 functions: main and m:

	level7@RainFall:~$ objdump -D -M intel-mnemonic level7 > /tmp/dump

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main