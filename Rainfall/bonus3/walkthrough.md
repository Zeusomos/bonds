# Preparations

Entering bonus3 home directory we see a bonus3 binary with sticky bit in access rights.
By objdumping the binary we see a main function:

	bonus3@RainFall:~$ objdump -D -M intel-mnemonic bonus3 > /tmp/dump_bon3

W