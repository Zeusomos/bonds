# Preparations

Entering bonus3 home directory we see a bonus3 binary with sticky bit in access rights.
By objdumping the binary we see a main function:

	bonus3@RainFall:~$ objdump -D -M intel-mnemonic bonus3 > /tmp/dump_bon3

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Exploitation

Here we have work with an argument and opportunity to use shell from the binary if a comparison is right. If we look on the binary and how buffer is filled in, we see that to some part of the buffer that shou