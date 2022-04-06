# Preparations

Entering level9 home directory we see a level9 binary with sticky bit in access rights.
By objdumping the binary we see only main function with classes:

	level9@RainFall:~$ objdump -D -M intel-mnemonic level9 > /tmp/dump9

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):
