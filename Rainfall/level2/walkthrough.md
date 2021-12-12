
# Preparations

Entering level2 home directory we see a level2 binary with sticky bit in access rights.
By objdumping the binary we see 2 functions: main and p:

	level2@RainFall:~$ objdump -D -M intel-mnemonic level2 > /tmp/dump